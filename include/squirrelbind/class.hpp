#pragma once
#ifndef SQUIRREL_BIND_CLASS_HEADER_H
#define SQUIRREL_BIND_CLASS_HEADER_H

#include <functional>
#include "function.hpp"
#include "binding.hpp"

namespace SquirrelBind {
    /**
    * @brief Squirrel class object
    */
    class SQBIND_API SqClass : public SqObject {
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
        * @brief Creates an empty invalid class
        */
		SqClass();
        /**
        * @brief Creates a new empty class
        */
        SqClass(HSQUIRRELVM vm);
        /**
        * @brief Converts SqObject to class object
        * @throws SqTypeException if the SqObject is not type of a class
        */
        explicit SqClass(const SqObject& object);
        /**
        * @brief Copy constructor
        */
        SqClass(const SqClass& other);
        /**
        * @brief Move constructor
        */
        SqClass(SqClass&& other) NOEXCEPT;
        /**
        * @brief Swaps two classes
        */
        void swap(SqClass& other) NOEXCEPT;
        /**
        * @brief Finds a function in this class
        * @throws SqRuntimeException if VM is invalid
        * @throws SqNotFoundException if function was not found
        * @throws SqTypeException if the object found is not a function
        */
        SqFunction findFunc(const char* name) const;
        /**
        * @brief Adds a new function type to this class
        * @param name Name of the function to add
        * @param func std::function that contains "this" pointer to the class type followed
        * by any number of arguments with any type
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
        template <typename Return, typename Object, typename... Args>
        SqFunction addFunc(const char* name, const std::function<Return(Object*, Args...)>& func, bool isStatic = false) {
            if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
            SqFunction ret(vm);
            sq_pushobject(vm, obj);
            detail::addMemberFunc(vm, name, func, isStatic);
            sq_pop(vm, 1);
            return ret;
        }
        /**
        * @brief Adds a new function type to this class
        * @param name Name of the function to add
        * @param memfunc Pointer to member function
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
        template <typename Return, typename Object, typename... Args>
        SqFunction addFunc(const char* name, Return(Object::*memfunc)(Args...), bool isStatic = false) {
            auto func = std::function<Return(Object*, Args...)>(std::mem_fn(memfunc));
            return addFunc(name, func, isStatic);
        }
        /**
        * @brief Adds a new function type to this class
        * @param name Name of the function to add
        * @param memfunc Pointer to constant member function
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
        template <typename Return, typename Object, typename... Args>
        SqFunction addFunc(const char* name, Return(Object::*memfunc)(Args...) const, bool isStatic = false) {
            auto func = std::function<Return(Object*, Args...)>(std::mem_fn(memfunc));
            return addFunc(name, func, isStatic);
        }
        /**
        * @brief Adds a new function type to this class
        * @param name Name of the function to add
        * @param lambda Lambda function that contains "this" pointer to the class type followed
        * by any number of arguments with any type
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
        template<typename F>
        SqFunction addFunc(const char* name, const F& lambda, bool isStatic = false) {
            return addFunc(name, detail::make_function(lambda), isStatic);
        }
        template<typename T, typename V>
        void addVar(const std::string& name, V T::* ptr, bool isStatic = false) {
            findTable("_get", tableGet, dlgGetStub);
            findTable("_set", tableSet, dlgSetStub);

            bindVar<T, V>(name, ptr, tableGet.getRaw(), varGetStub<T, V>, isStatic);
            bindVar<T, V>(name, ptr, tableSet.getRaw(), varSetStub<T, V>, isStatic);
        }
        template<typename T, typename V>
        void addConstVar(const std::string& name, V T::* ptr, bool isStatic = false) {
            findTable("_get", tableGet, dlgGetStub);

            bindVar<T, V>(name, ptr, tableGet.getRaw(), varGetStub<T, V>, isStatic);
        }
        /**
        * @brief Copy assingment operator
        */
        SqClass& operator = (const SqClass& other);
        /**
        * @brief Move assingment operator
        */
        SqClass& operator = (SqClass&& other) NOEXCEPT;
    protected:
        void findTable(const char* name, SqObject& table, SQFUNCTION dlg) const;
        static SQInteger dlgGetStub(HSQUIRRELVM vm);
        static SQInteger dlgSetStub(HSQUIRRELVM vm);

        template<typename T, typename V>
        void bindVar(const std::string& name, V T::* ptr, HSQOBJECT& table, SQFUNCTION stub, bool isStatic) {
            auto rst = sq_gettop(vm);

            sq_pushobject(vm, table);
            sq_pushstring(vm, name.c_str(), name.size());

            auto vp = sq_newuserdata(vm, sizeof(ptr));
            std::memcpy(vp, &ptr, sizeof(ptr));

            sq_newclosure(vm, stub, 1);

            if (SQ_FAILED(sq_newslot(vm, -3, isStatic))) {
                throw SqTypeException("Failed to bind member variable to class");
            }

            sq_pop(vm, 1);
            sq_settop(vm, rst);
        }

        template<typename T, typename V>
        static SQInteger varGetStub(HSQUIRRELVM vm) {
            T* ptr;
            sq_getinstanceup(vm, 1, reinterpret_cast<SQUserPointer*>(&ptr), nullptr);

            typedef V T::*M;
            M* memberPtr = nullptr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&memberPtr), nullptr);
            M member = *memberPtr;

            detail::push(vm, ptr->*member);
            return 1;
        }

        template<typename T, typename V>
        static SQInteger varSetStub(HSQUIRRELVM vm) {
            T* ptr;
            sq_getinstanceup(vm, 1, reinterpret_cast<SQUserPointer*>(&ptr), nullptr);

            typedef V T::*M;
            M* memberPtr = nullptr;
            sq_getuserdata(vm, -1, reinterpret_cast<SQUserPointer*>(&memberPtr), nullptr);
            M member = *memberPtr;

            ptr->*member = detail::pop<V>(vm, 2);
            return 0;
        }

        SqObject tableSet;
        SqObject tableGet;
    };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline SqClass popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_CLASS);
            SqClass val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw SqTypeException("Could not get SqClass from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif