#pragma once
#ifndef SQUIRREL_BIND_CLASS_HEADER_H
#define SQUIRREL_BIND_CLASS_HEADER_H

#include <functional>
#include "binding.hpp"

namespace SquirrelBind {
    class SqVM;
    /**
    * @brief
    */
    template<class T>
    class SqClassWrapper {
    public:
        static void sqExposeClass(SqVM& vm) {
            SqObject exposedClass = T::expose(vm);
            classObj = exposedClass.get();
        }
        static HSQOBJECT& sqGetClass() {
            return classObj;
        }
    private:
        static HSQOBJECT classObj;
    };

    template<class T>
    HSQOBJECT SqClassWrapper<T>::classObj;

    /**
    * @brief Squirrel class object
    */
    class SqClass: public SqObject {
    public:
        /**
        * @brief Constructor helper class
        */
        template<class Signature>
        struct Ctor;

        template<class T, class... Args>
        struct Ctor<T(Args...)> {
            static T* allocate(Args&&... args) {
                return new T(std::forward<Args>(args)...);
            }
        };
        /**
        * @brief Creates a new empty class
        */
        explicit SqClass(HSQUIRRELVM vm);
        /**
        * @brief Converts SqObject to class object
        * @throws SqTypeException if the SqObject is not type of a class
        */
        explicit SqClass(const SqObject& object);
        /**
        * @brief Copy constructor
        */
        explicit SqClass(const SqClass& other);
        /**
        * @brief Move constructor
        */
        SqClass(SqClass&& other) NOEXCEPT;
        /**
        * @brief Finds a function in this class
        * @throws SqRuntimeException if VM is invalid
        * @throws SqNotFoundException if function was not found
        * @throws SqTypeException if the object found is not a function
        */
        SqFunction findFunc(const char* name) const;
		/**
        * @brief Adds a new function type to this class
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
		template <typename Return, typename Object, typename... Args>
		SqFunction addFunc(const char* name, Return (Object::*memfunc)(Args...)) {
			if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
			auto func = std::function<Return (Object*, Args...)>(std::mem_fn(memfunc));
			SqFunction ret(vm);
            sq_pushobject(vm, obj);
            detail::addMemberFunc(vm, name, func);
            sq_pop(vm, 1);
            return ret;
		}
		template <typename Return, typename Object, typename... Args>
		SqFunction addFunc(const char* name, Return (Object::*memfunc)(Args...) const) {
			if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
			auto func = std::function<Return (Object*, Args...)>(std::mem_fn(memfunc));
			SqFunction ret(vm);
            sq_pushobject(vm, obj);
            detail::addMemberFunc(vm, name, func);
            sq_pop(vm, 1);
            return ret;
		}
        /**
        * @brief Copy assingment operator
        */
        SqClass& operator = (const SqClass& other);
        /**
        * @brief Move assingment operator
        */
        SqClass& operator = (SqClass&& other) NOEXCEPT;
    };

	namespace detail {
		template<typename T>
		struct pushByCopy<SqClassWrapper<T>> {
			static void f(HSQUIRRELVM vm, const SqClassWrapper<T>& value) {
				throw SqTypeException("Cannot push SqClassWrapper, type not supported!");
			}
		};
	}
}

#endif