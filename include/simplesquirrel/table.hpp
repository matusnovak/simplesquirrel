#pragma once
#ifndef SSQ_TABLE_HEADER_H
#define SSQ_TABLE_HEADER_H

#include "class.hpp"

namespace ssq {
    class Enum;
    /**
    * @brief Squirrel table object
    * @ingroup simplesquirrel
    */
    class SSQ_API Table: public Object {
    public:
        /**
        * @brief Creates empty table with null VM
        * @note This object will be unusable
        */
        Table();
        /**
        * @brief Destructor
        */
        virtual ~Table() = default;
        /**
        * @brief Converts Object to Table
        * @throws TypeException if the Object is not type of a table
        */
        explicit Table(const Object& other);
        /**
        * @brief Creates empty table
        */
        explicit Table(HSQUIRRELVM vm);
        /**
        * @brief Copy constructor
        */
        Table(const Table& other);
        /**
        * @brief Move constructor
        */
        Table(Table&& other) NOEXCEPT;
        /**
        * @brief Finds a function in this table
        * @throws RuntimeException if VM is invalid
        * @throws NotFoundException if function was not found
        * @throws TypeException if the object found is not a function
        * @returns Function object references the found function
        */
        Function findFunc(const char* name) const;
        /**
        * @brief Finds a class in this table
        * @throws NotFoundException if function was not found
        * @throws TypeException if the object found is not a function
        * @returns Class object references the found class
        */
        Class findClass(const char* name) const;
        /**
        * @brief Adds a new class type to this table
        * @returns Class object references the added class
        */
        template<typename T, typename... Args>
        Class addClass(const char* name, const std::function<T*(Args...)>& allocator = std::bind(&detail::defaultClassAllocator<T>), bool release = true){
            sq_pushobject(vm, obj);
            Class cls(detail::addClass(vm, name, allocator, release));
            sq_pop(vm, 1);
            return cls;
        }
        /**
        * @brief Adds a new class type to this table
        * @returns Class object references the added class
        */
        template<typename T, typename... Args>
        Class addClass(const char* name, const Class::Ctor<T(Args...)>& constructor, bool release = true){
            const std::function<T*(Args...)> func = &constructor.allocate;
            return addClass<T>(name, func, release);
        }
        /**
        * @brief Adds a new class type to this table
        * @returns Class object references the added class
        */
        template<typename F>
        Class addClass(const char* name, const F& lambda, bool release = true) {
            return addClass(name, detail::make_function(lambda), release);
        }
        /**
        * @brief Adds a new abstract class type to this table
        * @returns Class object references the added class
        */
        template<typename T>
        Class addAbstractClass(const char* name) {
            sq_pushobject(vm, obj);
            Class cls(detail::addAbstractClass<T>(vm, name));
            sq_pop(vm, 1);
            return cls;
        }
        /**
        * @brief Adds a new function type to this table
        * @returns Function object references the added function
        */
        template<typename R, typename... Args>
        Function addFunc(const char* name, const std::function<R(Args...)>& func){
            Function ret(vm);
            sq_pushobject(vm, obj);
            detail::addFunc(vm, name, func);
            sq_pop(vm, 1);
            return ret;
        }
        /**
        * @brief Adds a new lambda type to this table
        * @returns Function object that references the added function
        */
        template<typename F>
        Function addFunc(const char* name, const F& lambda) {
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
        Table addTable(const char* name);
        /**
        * @brief Copy assingment operator
        */
        Table& operator = (const Table& other);
        /**
        * @brief Move assingment operator
        */
        Table& operator = (Table&& other) NOEXCEPT;
    };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline Table popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_TABLE);
            Table val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Table from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif