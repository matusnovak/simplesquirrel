#include "../include/simplesquirrel/instance.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include "../include/simplesquirrel/class.hpp"
#include <squirrel.h>
#include <forward_list>

namespace ssq {
    Instance::Instance():Object() {
            
    }

    Instance::Instance(HSQUIRRELVM vm):Object(vm) {
            
    }

    Instance::Instance(const Object& object):Object(object) {
        if (object.getType() != Type::INSTANCE) throw TypeException("bad cast", "INSTANCE", object.getTypeStr());
    }

    Instance::Instance(const Instance& other):Object(other) {
            
    }

    Instance::Instance(Instance&& other) NOEXCEPT :Object(std::forward<Instance>(other)) {
            
    }

    Class Instance::getClass() {
        Class cls(vm);
        sq_pushobject(vm, obj);
        if(SQ_FAILED(sq_getclass(vm, -1))) {
            throw TypeException("Failed to get class from instance");
        }
        sq_getstackobj(vm, -1, &cls.getRaw());
        sq_addref(vm, &cls.getRaw());
        sq_pop(vm, 1);
        return cls;
    }

    Instance& Instance::operator = (const Instance& other){
        Object::operator = (other);
        return *this;
    }

    Instance& Instance::operator = (Instance&& other) NOEXCEPT {
        Object::operator = (std::forward<Instance>(other));
        return *this;
    }

    SqWeakRef::SqWeakRef():Instance() {
        weak = true;
    }

    SqWeakRef::SqWeakRef(HSQUIRRELVM vm):Instance(vm) {
        weak = true;
    }

    SqWeakRef::SqWeakRef(const SqWeakRef& other):Instance(other) {
    }

    SqWeakRef::SqWeakRef(const Instance& instance): Instance(instance.getHandle()) {
        weak = true;
        obj = instance.getRaw();
    }

    SqWeakRef::SqWeakRef(SqWeakRef&& other):Instance() {
        Instance::swap(other);
    }

    void SqWeakRef::swap(SqWeakRef& other) {
        Instance::swap(other);
    }

    SqWeakRef& SqWeakRef::operator = (const SqWeakRef& other){
        Instance::operator= (other);
        return *this;
    }

    SqWeakRef& SqWeakRef::operator = (SqWeakRef&& other){
        Instance::operator= (std::forward<SqWeakRef>(other));
        return *this;
    }
}