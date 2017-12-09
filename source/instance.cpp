#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include "../include/squirrelbind/class.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqInstance::SqInstance(HSQUIRRELVM vm):SqObject(vm) {
            
    }

    SqInstance::SqInstance(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::INSTANCE) throw SqTypeException("bad cast", "INSTANCE", object.getTypeStr());
    }

    SqInstance::SqInstance(const SqInstance& other):SqObject(other) {
            
    }

    SqInstance::SqInstance(SqInstance&& other) NOEXCEPT :SqObject(std::forward<SqInstance>(other)) {
            
    }

    SqClass SqInstance::getClass() {
        SqClass cls(vm);
        sq_pushobject(vm, obj);
        if(SQ_FAILED(sq_getclass(vm, -1))) {
            throw SqTypeException("Failed to get class from instance");
        }
        sq_getstackobj(vm, -1, &cls.getRaw());
        sq_addref(vm, &cls.getRaw());
        sq_pop(vm, 1);
        return cls;
    }

    SqInstance& SqInstance::operator = (const SqInstance& other){
        SqObject::operator = (other);
        return *this;
    }

    SqInstance& SqInstance::operator = (SqInstance&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqInstance>(other));
        return *this;
    }
}