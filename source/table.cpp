#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/enum.hpp"
#include "../include/squirrelbind/table.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqTable::SqTable():SqObject() {
            
    }

    SqTable::SqTable(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::TABLE) throw SqTypeException("bad cast", "TABLE", object.getTypeStr());
    }

    SqTable::SqTable(HSQUIRRELVM vm):SqObject(vm) {
        sq_newtable(vm);
        sq_getstackobj(vm, -1, &obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1); // Pop table
    }

    SqTable::SqTable(const SqTable& other):SqObject(other) {
            
    }

    SqTable::SqTable(SqTable&& other) NOEXCEPT :SqObject(std::forward<SqTable>(other)) {
            
    }

    SqFunction SqTable::findFunc(const char* name) const {
        SqObject object = SqObject::find(name);
        return SqFunction(object);
    }

    SqClass SqTable::findClass(const char* name) const {
        SqObject object = SqObject::find(name);
        return SqClass(object);
    }

    SqTable SqTable::addTable(const char* name) {
        SqTable table(vm);
        sq_pushobject(vm, obj);
        sq_pushstring(vm, name, strlen(name));
        detail::push<SqObject>(vm, table);
        sq_newslot(vm, -3, false);
        sq_pop(vm,1); // pop table
        return std::move(table);
    }

    size_t SqTable::size() {
        sq_pushobject(vm, obj);
        SQInteger s = sq_getsize(vm, -1);
        sq_pop(vm, 1);
        return static_cast<size_t>(s);
    }

    SqTable& SqTable::operator = (const SqTable& other){
        SqObject::operator = (other);
        return *this;
    }

    SqTable& SqTable::operator = (SqTable&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqTable>(other));
        return *this;
    }
}
