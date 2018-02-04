#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/args.hpp"
#include "../include/simplesquirrel/class.hpp"
#include "../include/simplesquirrel/instance.hpp"
#include "../include/simplesquirrel/table.hpp"
#include "../include/simplesquirrel/function.hpp"
#include "../include/simplesquirrel/enum.hpp"
#include "../include/simplesquirrel/array.hpp"
#include <squirrel.h>

namespace ssq {
    namespace detail {
        void pushRaw(HSQUIRRELVM vm, const Object& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Class& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Instance& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Table& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Function& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Enum& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const Array& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqWeakRef& value) {
            sq_pushobject(vm, value.getRaw());
        }
    }
}