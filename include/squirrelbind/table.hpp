#pragma once
#ifndef SQUIRREL_BIND_TABLE_HEADER_H
#define SQUIRREL_BIND_TABLE_HEADER_H

#include "class.hpp"

namespace SquirrelBind {
    class SqEnum;
    /**
    * @brief Squirrel table object
    */
    class SQBIND_API SqTable: public SqObject {
    public:
        /**
        * @brief Creates empty table with null VM
        * @note This object will be unusable
        */
        SqTable();
        /**
        * @brief Converts SqObject to SqTable
        * @throws SqTypeException if the SqObject is not type of a table
        */
        explicit SqTable(const SqObject& other);
        /**
        * @brief Creates empty table
        */
        explicit SqTable(HSQUIRRELVM vm);
        /**
        * @brief Copy constructor
        */
        SqTable(const SqTable& other);
        /**
        * @brief Move constructor
        */
        SqTable(SqTable&& other) NOEXCEPT;
        /**
        * @brief Finds a function in this table
        * @throws SqRuntimeException if VM is invalid
        * @throws SqNotFoundException if function was not found
        * @throws SqTypeException if the object found is not a function
        * @returns SqFunction object references the found function
        */
        SqFunction findFunc(const char* name) const;
        /**
        * @brief Finds a class in this table
        * @throws SqNotFoundException if function was not found
        * @throws SqTypeException if the object found is not a function
        * @returns SqClass object references the found class
        */
        SqClass findClass(const char* name) const;
        /**
        * @brief Adds a new class type to this table
        * @returns SqClass object references the added class
        */
        template<typename T, typename... Args>
        SqClass addClass(const char* name, const std::function<T*(Args...)>& allocator = std::bind(&detail::defaultClassAllocator<T>), bool release = true){
            sq_pushobject(vm, obj);
            SqClass cls(detail::addClass(vm, name, allocator, release));
            sq_pop(vm, 1);
            return cls;
        }
        /**
        * @brief Adds a new class type to this table
        * @returns SqClass object references the added class
        */
        template<typename T, typename... Args>
        SqClass addClass(const char* name, const SqClass::Ctor<T(Args...)>& constructor, bool release = true){
            const std::function<T*(Args...)> func = &constructor.allocate;
            return addClass<T>(name, func, release);
        }
        /**
        * @brief Adds a new class type to this table
        * @returns SqClass object references the added class
        */
        template<typename F>
        SqClass addClass(const char* name, const F& lambda, bool release = true) {
            return addClass(name, detail::make_function(lambda), release);
        }
        /**
        * @brief Adds a new abstract class type to this table
        * @returns SqClass object references the added class
        */
        template<typename T>
        SqClass addAbstractClass(const char* name) {
            sq_pushobject(vm, obj);
            SqClass cls(detail::addAbstractClass<T>(vm, name));
            sq_pop(vm, 1);
            return cls;
        }
        /**
        * @brief Adds a new function type to this table
        * @returns SqFunction object references the added function
        */
        template<typename R, typename... Args>
        SqFunction addFunc(const char* name, const std::function<R(Args...)>& func){
            SqFunction ret(vm);
            sq_pushobject(vm, obj);
            detail::addFunc(vm, name, func);
            sq_pop(vm, 1);
            return ret;
        }
        /**
        * @brief Adds a new lambda type to this table
        * @returns SqFunction object that references the added function
        */
        template<typename F>
        SqFunction addFunc(const char* name, const F& lambda) {
            return addFunc(name, detail::make_function(lambda));
        }
        /**
         * @brief Adds a new key-value pair to this table
         */
        template<typename T>
        inline void set(const char* name, const T& value) {
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name, strlen(name));
            detail::push<T>(vm, value);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
        }
        template<typename T>
        inline T get(const char* name) {
            return find(name).to<T>();
        }
        size_t size();
        /**
         * @brief Adds a new table to this table
         */
        SqTable addTable(const char* name);
        /**
        * @brief Copy assingment operator
        */
        SqTable& operator = (const SqTable& other);
        /**
        * @brief Move assingment operator
        */
        SqTable& operator = (SqTable&& other) NOEXCEPT;
    };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline SqTable popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_TABLE);
            SqTable val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw SqTypeException("Could not get SqTable from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif