#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/enum.hpp"
#include "../include/squirrelbind/vm.hpp"
#include <squirrel.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>
#include <sqstdmath.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <forward_list>
#include <cstdarg>
#include <cstring>
#include <iostream>

namespace SquirrelBind {
    SqVM::SqVM(size_t stackSize, SqLibs::Flag flags):SqTable() {
        vm = sq_open(stackSize);
        sq_resetobject(&obj);
        sq_setforeignptr(vm, this);

        registerStdlib(flags);

        setPrintFunc(&SqVM::defaultPrintFunc, &SqVM::defaultErrorFunc);
        setRuntimeErrorFunc(&SqVM::defaultRuntimeErrorFunc);
        setCompileErrorFunc(&SqVM::defaultCompilerErrorFunc);

        sq_pushroottable(vm);
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
    }

    void SqVM::destroy() {
        if (vm != nullptr) {
            sq_resetobject(&obj);
            sq_close(vm);
        }
        vm = nullptr;
    }

    SqVM::~SqVM() {
        destroy();
    }

    void SqVM::swap(SqVM& other) NOEXCEPT {
        using std::swap;
        SqObject::swap(other);
        swap(runtimeException, other.runtimeException);
        swap(compileException, other.compileException);
        if(vm != nullptr) {
            sq_setforeignptr(vm, this);
        }
        if(other.vm != nullptr) {
            sq_setforeignptr(other.vm, this);
        }
    }
        
    SqVM::SqVM(SqVM&& other) NOEXCEPT :SqTable() {
        swap(other);
    }

    void SqVM::registerStdlib(SqLibs::Flag flags) {
        if (flags == 0)return;
        sq_pushroottable(vm);
        if(flags & SqLibs::IO)
            sqstd_register_iolib(vm);
        if(flags & SqLibs::BLOB)
            sqstd_register_bloblib(vm);
        if(flags & SqLibs::MATH)
            sqstd_register_mathlib(vm);
        if(flags & SqLibs::SYSTEM)
            sqstd_register_systemlib(vm);
        if(flags & SqLibs::STRING)
            sqstd_register_stringlib(vm);
        sq_pop(vm, 1);
    }

    void SqVM::setPrintFunc(SqPrintFunc printFunc, SqErrorFunc errorFunc) {
        sq_setprintfunc(vm, printFunc, errorFunc);
    }

    void SqVM::setRuntimeErrorFunc(SqRuntimeErrorFunc runtimeErrorFunc) {
        sq_newclosure(vm, runtimeErrorFunc, 0);
        sq_seterrorhandler(vm);
    }

    void SqVM::setCompileErrorFunc(SqCompileErrorFunc compileErrorFunc) {
        sq_setcompilererrorhandler(vm, compileErrorFunc);
    }

    SQInteger SqVM::getTop() const {
        return sq_gettop(vm);
    }

    SqScript SqVM::compileSource(const char* source, const char* name) {
        SqScript script(vm);
        if(SQ_FAILED(sq_compilebuffer(vm, source, strlen(source), name, true))){
            if (!compileException)throw SqCompileException("Source cannot be compiled!");
            throw *compileException;
        }

        sq_getstackobj(vm,-1,&script.getRaw());
        sq_addref(vm, &script.getRaw());
        sq_pop(vm, 1);
        return script;
    }

    SqScript SqVM::compileFile(const char* path) {
        SqScript script(vm);
        if (SQ_FAILED(sqstd_loadfile(vm, path, true))) {
            if (!compileException)throw SqCompileException("File not found or cannot be read!");
            throw *compileException;
        }

        sq_getstackobj(vm, -1, &script.getRaw());
        sq_addref(vm, &script.getRaw());
        sq_pop(vm, 1);
        return script;
    }

    void SqVM::run(const SqScript& script) const {
        if(!script.isEmpty()) {
            SQInteger top = sq_gettop(vm);
            sq_pushobject(vm, script.getRaw());
            sq_pushroottable(vm);
            SQRESULT result = sq_call(vm, 1, false, true);
            sq_settop(vm, top);
            if(SQ_FAILED(result)){
                throw *runtimeException;
            }
        }
        else {
            throw SqRuntimeException("Empty script object");
        }
    }

    SqEnum SqVM::addEnum(const char* name) {
        SqEnum enm(vm);
        sq_pushconsttable(vm);
        sq_pushstring(vm, name, strlen(name));
        detail::push<SqObject>(vm, enm);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
        return std::move(enm);
    }

    SqVM& SqVM::operator = (SqVM&& other) NOEXCEPT {
        if(this != &other) {
            swap(other);
        }
        return *this;
    }

    SqObject SqVM::callAndReturn(SQUnsignedInteger nparams, SQInteger top) const {
        if(SQ_FAILED(sq_call(vm, 1 + nparams, true, true))){
            sq_settop(vm, top);
            if (runtimeException == nullptr)
                throw SqRuntimeException("Unknown squirrel runtime error");
            throw *runtimeException;
        }
            
        SqObject ret(vm);
        sq_getstackobj(vm, -1, &ret.getRaw());
        sq_addref(vm, &ret.getRaw());
        sq_settop(vm, top);
        return ret;
    }

    void SqVM::debugStack() const {
        auto top = getTop();
        while(top >= 0) {
            SQObjectType objectType = sq_gettype(vm, top);
            SqType type = SqType(objectType);
            std::cout << "stack index: " << top << " type: " << sqTypeToStr(type) << std::endl;
            top--;
        }
    }

    void SqVM::defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        printf("\n");
        va_end(vl);
    }

    void SqVM::defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        fprintf(stderr, s, vl);
        fprintf(stderr, "\n");
        va_end(vl);
    }

    SQInteger SqVM::defaultRuntimeErrorFunc(HSQUIRRELVM vm) {
        SQStackInfos si;
        sq_stackinfos(vm, 1, &si);

        auto source = (si.source != nullptr ? si.source : "null");
        auto funcname = (si.funcname != nullptr ? si.funcname : "unknown");

        const SQChar *sErr = 0;
        if(sq_gettop(vm) >= 1){
            if(SQ_FAILED(sq_getstring(vm, 2, &sErr))){
                sErr = "unknown error";
            }
        }

        auto ptr = reinterpret_cast<SqVM*>(sq_getforeignptr(vm));
        ptr->runtimeException.reset(new SqRuntimeException(
            sErr,
            source,
            funcname,
            si.line
        ));
        return 0;
    }

    void SqVM::defaultCompilerErrorFunc(
        HSQUIRRELVM vm,
        const SQChar* desc,
        const SQChar* source,
        SQInteger line,
        SQInteger column) {
        auto ptr = reinterpret_cast<SqVM*>(sq_getforeignptr(vm));
        ptr->compileException.reset(new SqCompileException(
            desc,
            source,
            line,
            column
        ));
    }

    void SqVM::pushArgs() {

    }
}
