#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/script.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
    Script::Script(HSQUIRRELVM vm) :Object(vm) {

    }

    void Script::swap(Script& other) NOEXCEPT {
        Object::swap(other);
    }

    Script::Script(Script&& other) NOEXCEPT :Object(std::forward<Object>(other)) {

    }

    Script& Script::operator = (Script&& other) NOEXCEPT {
        Object::operator = (std::forward<Script>(other));
        return *this;
    }
}
