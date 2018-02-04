#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/enum.hpp"
#include "../include/simplesquirrel/vm.hpp"
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

namespace ssq {
    VM::VM(size_t stackSize, Libs::Flag flags):Table() {
        vm = sq_open(stackSize);
        sq_resetobject(&obj);
        sq_setforeignptr(vm, this);

        registerStdlib(flags);

        setPrintFunc(&VM::defaultPrintFunc, &VM::defaultErrorFunc);
        setRuntimeErrorFunc(&VM::defaultRuntimeErrorFunc);
        setCompileErrorFunc(&VM::defaultCompilerErrorFunc);

        sq_pushroottable(vm);
        sq_getstackobj(vm,-1,&obj);
        sq_addref(vm, &obj);
        sq_pop(vm, 1);
    }

    void VM::destroy() {
		classMap.clear();
        if (vm != nullptr) {
            sq_resetobject(&obj);
            sq_close(vm);
        }
        vm = nullptr;
    }

    VM::~VM() {
        destroy();
    }

    void VM::swap(VM& other) NOEXCEPT {
        using std::swap;
        Object::swap(other);
        swap(runtimeException, other.runtimeException);
        swap(compileException, other.compileException);
		swap(classMap, other.classMap);

        if(vm != nullptr) {
            sq_setforeignptr(vm, this);
        }
        if(other.vm != nullptr) {
            sq_setforeignptr(other.vm, this);
        }
    }
        
    VM::VM(VM&& other) NOEXCEPT :Table() {
        swap(other);
    }

    void VM::registerStdlib(Libs::Flag flags) {
        if (flags == 0)return;
        sq_pushroottable(vm);
        if(flags & ssq::Libs::IO)
            sqstd_register_iolib(vm);
        if(flags & ssq::Libs::BLOB)
            sqstd_register_bloblib(vm);
        if(flags & ssq::Libs::MATH)
            sqstd_register_mathlib(vm);
        if(flags & ssq::Libs::SYSTEM)
            sqstd_register_systemlib(vm);
        if(flags & ssq::Libs::STRING)
            sqstd_register_stringlib(vm);
        sq_pop(vm, 1);
    }

    void VM::setPrintFunc(SqPrintFunc printFunc, SqErrorFunc errorFunc) {
        sq_setprintfunc(vm, printFunc, errorFunc);
    }

    void VM::setRuntimeErrorFunc(SqRuntimeErrorFunc runtimeErrorFunc) {
        sq_newclosure(vm, runtimeErrorFunc, 0);
        sq_seterrorhandler(vm);
    }

    void VM::setCompileErrorFunc(SqCompileErrorFunc compileErrorFunc) {
        sq_setcompilererrorhandler(vm, compileErrorFunc);
    }

    SQInteger VM::getTop() const {
        return sq_gettop(vm);
    }

    Script VM::compileSource(const char* source, const char* name) {
        Script script(vm);
        if(SQ_FAILED(sq_compilebuffer(vm, source, strlen(source), name, true))){
            if (!compileException)throw CompileException("Source cannot be compiled!");
            throw *compileException;
        }

        sq_getstackobj(vm,-1,&script.getRaw());
        sq_addref(vm, &script.getRaw());
        sq_pop(vm, 1);
        return script;
    }

    Script VM::compileFile(const char* path) {
        Script script(vm);
        if (SQ_FAILED(sqstd_loadfile(vm, path, true))) {
            if (!compileException)throw CompileException("File not found or cannot be read!");
            throw *compileException;
        }

        sq_getstackobj(vm, -1, &script.getRaw());
        sq_addref(vm, &script.getRaw());
        sq_pop(vm, 1);
        return script;
    }

    void VM::run(const Script& script) const {
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
            throw RuntimeException("Empty script object");
        }
    }

    Enum VM::addEnum(const char* name) {
        Enum enm(vm);
        sq_pushconsttable(vm);
        sq_pushstring(vm, name, strlen(name));
        detail::push<Object>(vm, enm);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
        return std::move(enm);
    }

    VM& VM::operator = (VM&& other) NOEXCEPT {
        if(this != &other) {
            swap(other);
        }
        return *this;
    }

    Object VM::callAndReturn(SQUnsignedInteger nparams, SQInteger top) const {
        if(SQ_FAILED(sq_call(vm, 1 + nparams, true, true))){
            sq_settop(vm, top);
            if (runtimeException == nullptr)
                throw RuntimeException("Unknown squirrel runtime error");
            throw *runtimeException;
        }
            
        Object ret(vm);
        sq_getstackobj(vm, -1, &ret.getRaw());
        sq_addref(vm, &ret.getRaw());
        sq_settop(vm, top);
        return ret;
    }

    void VM::debugStack() const {
        auto top = getTop();
        while(top >= 0) {
            SQObjectType objectType = sq_gettype(vm, top);
            Type type = Type(objectType);
            std::cout << "stack index: " << top << " type: " << typeToStr(type) << std::endl;
            top--;
        }
    }

    void VM::defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        vprintf(s, vl);
        printf("\n");
        va_end(vl);
    }

    void VM::defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        va_list vl;
        va_start(vl, s);
        fprintf(stderr, s, vl);
        fprintf(stderr, "\n");
        va_end(vl);
    }

    SQInteger VM::defaultRuntimeErrorFunc(HSQUIRRELVM vm) {
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

        auto ptr = reinterpret_cast<VM*>(sq_getforeignptr(vm));
        ptr->runtimeException.reset(new RuntimeException(
            sErr,
            source,
            funcname,
            si.line
        ));
        return 0;
    }

    void VM::defaultCompilerErrorFunc(
        HSQUIRRELVM vm,
        const SQChar* desc,
        const SQChar* source,
        SQInteger line,
        SQInteger column) {
        auto ptr = reinterpret_cast<VM*>(sq_getforeignptr(vm));
        ptr->compileException.reset(new CompileException(
            desc,
            source,
            line,
            column
        ));
    }

    void VM::pushArgs() {

    }

	void VM::addClassObj(size_t hashCode, const HSQOBJECT& obj) {
		classMap[hashCode] = obj;
	}

	const HSQOBJECT& VM::getClassObj(size_t hashCode) {
		return classMap.at(hashCode);
	}

	namespace detail {
	    void addClassObj(HSQUIRRELVM vm, size_t hashCode, const HSQOBJECT& obj) {
		    VM* machine = reinterpret_cast<VM*>(sq_getforeignptr(vm));
			machine->addClassObj(hashCode, obj);
	    }

		const HSQOBJECT& getClassObj(HSQUIRRELVM vm, size_t hashCode) {
		    VM* machine = reinterpret_cast<VM*>(sq_getforeignptr(vm));
			return machine->getClassObj(hashCode);
	    }
    }
}
