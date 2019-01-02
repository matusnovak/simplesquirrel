#include "../include/simplesquirrel/function.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
    Function::Function(HSQUIRRELVM vm):Object(vm) {
            
    }

    Function::Function(const Object& object):Object(object) {
        if (object.getType() != Type::CLOSURE && object.getType() != Type::NATIVECLOSURE) throw TypeException("bad cast", "CLOSURE", object.getTypeStr());
    }

    Function::Function(const Function& other):Object(other) {
            
    }

    Function::Function(Function&& other) NOEXCEPT :Object(std::forward<Function>(other)) {
            
    }

    unsigned int Function::getNumOfParams() const {
        SQInteger nparams;
        SQInteger nfreevars;
        sq_pushobject(vm, obj);
        if (SQ_FAILED(sq_getclosureinfo(vm, -1, &nparams, &nfreevars))) {
            sq_pop(vm, 1);
            throw TypeException("Get function info failed");
        }
        sq_pop(vm, 1);
        return nparams -1;
    }

    Function& Function::operator = (const Function& other){
        Object::operator = (other);
        return *this;
    }

    Function& Function::operator = (Function&& other) NOEXCEPT {
        Object::operator = (std::forward<Function>(other));
        return *this;
    }
}
