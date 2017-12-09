#include "../include/squirrelbind/enum.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqEnum::SqEnum():SqObject() {
            
    }

    SqEnum::SqEnum(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::TABLE) throw SqTypeException("bad cast", "TABLE", object.getTypeStr());
    }

    SqEnum::SqEnum(HSQUIRRELVM vm):SqObject(vm) {
        sq_newtable(vm);
        sq_getstackobj(vm, -1, &obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1); // Pop enum table
    }

    SqEnum::SqEnum(const SqEnum& other):SqObject(other) {
            
    }

    SqEnum::SqEnum(SqEnum&& other) NOEXCEPT :SqObject(std::forward<SqEnum>(other)) {
            
    }

    SqEnum& SqEnum::operator = (const SqEnum& other){
        SqObject::operator = (other);
        return *this;
    }

    SqEnum& SqEnum::operator = (SqEnum&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqEnum>(other));
        return *this;
    }
}
