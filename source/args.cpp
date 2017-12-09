#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/args.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/table.hpp"
#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/enum.hpp"
#include "../include/squirrelbind/array.hpp"
#include <squirrel.h>

namespace SquirrelBind {
    namespace detail {
        void pushRaw(HSQUIRRELVM vm, const SqObject& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqClass& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqInstance& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqTable& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqFunction& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqEnum& value) {
            sq_pushobject(vm, value.getRaw());
        }
        void pushRaw(HSQUIRRELVM vm, const SqArray& value) {
            sq_pushobject(vm, value.getRaw());
        }
    }
}