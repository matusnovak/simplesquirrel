#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/script.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqScript::SqScript(HSQUIRRELVM vm) :SqObject(vm) {

    }

    void SqScript::swap(SqScript& other) NOEXCEPT {
        SqObject::swap(other);
    }

    SqScript::SqScript(SqScript&& other) NOEXCEPT :SqObject(std::forward<SqObject>(other)) {

    }

    SqScript& SqScript::operator = (SqScript&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqScript>(other));
        return *this;
    }
}
