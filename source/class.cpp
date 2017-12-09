#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include "../include/squirrelbind/function.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqClass::SqClass(HSQUIRRELVM vm) :SqObject(vm), tableSet(vm), tableGet(vm) {

    }

    SqClass::SqClass(const SqObject& object) : SqObject(object.getHandle())
        , tableSet(object.getHandle()), tableGet(object.getHandle()) {

        if (object.getType() != SqType::CLASS) throw SqTypeException("bad cast", "CLASS", object.getTypeStr());
        if (vm != nullptr && !object.isEmpty()) {
            obj = object.getRaw();
            sq_addref(vm, &obj);
        }
    }

    SqClass::SqClass(const SqClass& other) :SqObject(other), tableSet(other.tableSet), tableGet(other.tableGet) {

    }

    SqClass::SqClass(SqClass&& other) NOEXCEPT : SqObject(std::forward<SqClass>(other)),
        tableSet(std::forward<SqObject>(other.tableSet)),
        tableGet(std::forward<SqObject>(other.tableGet)) {

    }

    void SqClass::swap(SqClass& other) NOEXCEPT {
        if (this != &other) {
            SqObject::swap(other);
            tableSet.swap(other.tableSet);
            tableGet.swap(other.tableGet);
        }
    }

    SqFunction SqClass::findFunc(const char* name) const {
        SqObject object = SqObject::find(name);
        return SqFunction(object);
    }

    SqClass& SqClass::operator = (const SqClass& other) {
        if (this != &other) {
            SqClass o(other);
            swap(o);
        }
        return *this;
    }

    SqClass& SqClass::operator = (SqClass&& other) NOEXCEPT {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    void SqClass::findTable(const char* name, SqObject& table, SQFUNCTION dlg) const {
        // Check if the table has been referenced
        if(!table.isEmpty()) {
            return;
        }
            
        // Find the table
        sq_pushobject(vm, obj);
        sq_pushstring(vm, name, strlen(name));

        if (SQ_FAILED(sq_get(vm, -2))) {
            // Does not exists
            sq_pop(vm, 1);

            // Create one
            table = SqObject(vm);
            sq_newtable(vm);
            sq_getstackobj(vm, -1, &table.getRaw());
            sq_addref(vm, &table.getRaw());
            sq_pop(vm, 1);

            sq_pushobject(vm, obj); // Push class obj
            sq_pushstring(vm, name, strlen(name));
            sq_pushobject(vm, table.getRaw());
            sq_newclosure(vm, dlg, 1);

            if(SQ_FAILED(sq_newslot(vm, -3, false))) {
                throw SqTypeException("Failed to create table!");
            }

            sq_pop(vm, 1); // Pop class obj

        } else {
            // Return one
            table = SqObject(vm);
            sq_getstackobj(vm, -1, &table.getRaw());
            sq_addref(vm, &table.getRaw());
            sq_pop(vm, 2);
        }
    }

    SQInteger SqClass::dlgGetStub(HSQUIRRELVM vm) {
        // Find the set method in the set table
        sq_push(vm, 2);
        if (!SQ_SUCCEEDED(sq_get(vm, -2))) {
            const SQChar* s;
            sq_getstring(vm, 2, &s);
            return sq_throwerror(vm, "Variable not found");
        }

        // push 'this'
        sq_push(vm, 1);

        // Call the setter
        sq_call(vm, 1, SQTrue, SQTrue);
        return 1;
    }

    SQInteger SqClass::dlgSetStub(HSQUIRRELVM vm) {
        sq_push(vm, 2);
        if (!SQ_SUCCEEDED(sq_get(vm, -2))) {
            const SQChar* s;
            sq_getstring(vm, 2, &s);
            return sq_throwerror(vm, "Variable not found");
        }

        sq_push(vm, 1);

        // Call the getter
        sq_push(vm, 3);
        sq_call(vm, 2, SQTrue, SQTrue);
        return 1;
    }
}