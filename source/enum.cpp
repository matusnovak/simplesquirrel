#include "../include/simplesquirrel/enum.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
    Enum::Enum():Object() {
            
    }

    Enum::Enum(const Object& object):Object(object) {
        if (object.getType() != Type::TABLE) throw TypeException("bad cast", "TABLE", object.getTypeStr());
    }

    Enum::Enum(HSQUIRRELVM vm):Object(vm) {
        sq_newtable(vm);
        sq_getstackobj(vm, -1, &obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1); // Pop enum table
    }

    Enum::Enum(const Enum& other):Object(other) {
            
    }

    Enum::Enum(Enum&& other) NOEXCEPT :Object(std::forward<Enum>(other)) {
            
    }

    Enum& Enum::operator = (const Enum& other){
        Object::operator = (other);
        return *this;
    }

    Enum& Enum::operator = (Enum&& other) NOEXCEPT {
        Object::operator = (std::forward<Enum>(other));
        return *this;
    }
}
