#include "../include/squirrelbind/array.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
    SqArray::SqArray(HSQUIRRELVM vm, size_t len):SqObject(vm) {
        sq_newarray(vm, len);
        sq_getstackobj(vm, -1, &obj);
        sq_addref(vm, &obj);
        sq_pop(vm,1); // Pop array
    }

    SqArray::SqArray(const SqObject& object):SqObject(object) {
        if (object.getType() != SqType::ARRAY) throw SqTypeException("bad cast", "ARRAY", object.getTypeStr());
    }

    SqArray::SqArray(const SqArray& other):SqObject(other) {
            
    }

    SqArray::SqArray(SqArray&& other) NOEXCEPT :SqObject(std::forward<SqArray>(other)) {
            
    }

    size_t SqArray::size() {
        sq_pushobject(vm, obj);
        SQInteger s = sq_getsize(vm, -1);
        sq_pop(vm, 1);
        return static_cast<size_t>(s);
    }

    std::vector<SqObject> SqArray::convertRaw() {
        sq_pushobject(vm, obj);
        SQInteger s = sq_getsize(vm, -1);
        std::vector<SqObject> ret;
        ret.reserve(s);
        while(s--) {
            if(SQ_FAILED(sq_arraypop(vm, -1, true))) {
                sq_pop(vm, 1);
                throw SqTypeException("Failed to pop value from back of the array");
            }
            ret.push_back(detail::pop<SqObject>(vm, -1));
        }
        sq_pop(vm, 1);
        return ret;
    }

    void SqArray::pop() {
        sq_pushobject(vm, obj);
        auto s = sq_getsize(vm, -1);
        if(s == 0) {
            sq_pop(vm, 1);
            throw SqTypeException("Out of bounds");
        }

        if(SQ_FAILED(sq_arraypop(vm, -1, false))) {
            sq_pop(vm, 1);
            throw SqTypeException("Failed to pop value from back of the array");
        }
        sq_pop(vm, 1);
    }

    SqArray& SqArray::operator = (const SqArray& other){
        SqObject::operator = (other);
        return *this;
    }

    SqArray& SqArray::operator = (SqArray&& other) NOEXCEPT {
        SqObject::operator = (std::forward<SqArray>(other));
        return *this;
    }
}