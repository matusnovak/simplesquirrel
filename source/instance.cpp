#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqInstance::SqInstance(HSQUIRRELVM vm):SqObject(vm) {
            
    }

    SqInstance::SqInstance(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::INSTANCE) throw SqTypeException("Object is not an instance", object.getTypeStr(), "INSTANCE");
    }

    SqInstance::SqInstance(const SqInstance& other):SqObject(other) {
            
    }

    SqInstance::SqInstance(SqInstance&& other) NOEXCEPT :SqObject(std::forward<SqInstance>(other)) {
            
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