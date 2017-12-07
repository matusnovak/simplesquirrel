#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqFunction::SqFunction(HSQUIRRELVM vm):SqObject(vm) {
            
    }

    SqFunction::SqFunction(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::CLOSURE) throw SqTypeException("Object is not a function", object.getTypeStr(), "CLOSURE");
    }

    SqFunction::SqFunction(const SqFunction& other):SqObject(other) {
            
    }

    SqFunction::SqFunction(SqFunction&& other) NOEXCEPT :SqObject(std::forward<SqFunction>(other)) {
            
    }

    unsigned int SqFunction::getNumOfParams() const {
        SQUnsignedInteger nparams;
        SQUnsignedInteger nfreevars;
        sq_pushobject(vm, obj);
        if (SQ_FAILED(sq_getclosureinfo(vm, -1, &nparams, &nfreevars))) {
            sq_pop(vm, 1);
            throw SqTypeException("Get function info failed");
        }

        return nparams -1;
    }

    SqFunction& SqFunction::operator = (const SqFunction& other){
        SqObject::operator = (other);
        return *this;
    }

    SqFunction& SqFunction::operator = (SqFunction&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqFunction>(other));
        return *this;
    }
}