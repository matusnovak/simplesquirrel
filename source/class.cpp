#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/class.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include "../include/simplesquirrel/function.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
	Class::Class() :Object(), tableSet(), tableGet() {

    }

    Class::Class(HSQUIRRELVM vm) :Object(vm), tableSet(vm), tableGet(vm) {

    }

    Class::Class(const Object& object) : Object(object.getHandle())
        , tableSet(object.getHandle()), tableGet(object.getHandle()) {

        if (object.getType() != Type::CLASS) throw TypeException("bad cast", "CLASS", object.getTypeStr());
        if (vm != nullptr && !object.isEmpty()) {
            obj = object.getRaw();
            sq_addref(vm, &obj);
        }
    }

    Class::Class(const Class& other) :Object(other), tableSet(other.tableSet), tableGet(other.tableGet) {

    }

    Class::Class(Class&& other) NOEXCEPT : Object(std::forward<Class>(other)),
        tableSet(std::forward<Object>(other.tableSet)),
        tableGet(std::forward<Object>(other.tableGet)) {

    }

    void Class::swap(Class& other) NOEXCEPT {
        if (this != &other) {
            Object::swap(other);
            tableSet.swap(other.tableSet);
            tableGet.swap(other.tableGet);
        }
    }

    Function Class::findFunc(const char* name) const {
        Object object = Object::find(name);
        return Function(object);
    }

    Class& Class::operator = (const Class& other) {
        if (this != &other) {
            Class o(other);
            swap(o);
        }
        return *this;
    }

    Class& Class::operator = (Class&& other) NOEXCEPT {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    void Class::findTable(const char* name, Object& table, SQFUNCTION dlg) const {
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
            table = Object(vm);
            sq_newtable(vm);
            sq_getstackobj(vm, -1, &table.getRaw());
            sq_addref(vm, &table.getRaw());
            sq_pop(vm, 1);

            sq_pushobject(vm, obj); // Push class obj
            sq_pushstring(vm, name, strlen(name));
            sq_pushobject(vm, table.getRaw());
            sq_newclosure(vm, dlg, 1);

            if(SQ_FAILED(sq_newslot(vm, -3, false))) {
                throw TypeException("Failed to create table!");
            }

            sq_pop(vm, 1); // Pop class obj

        } else {
            // Return one
            table = Object(vm);
            sq_getstackobj(vm, -1, &table.getRaw());
            sq_addref(vm, &table.getRaw());
            sq_pop(vm, 2);
        }
    }

    SQInteger Class::dlgGetStub(HSQUIRRELVM vm) {
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

    SQInteger Class::dlgSetStub(HSQUIRRELVM vm) {
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