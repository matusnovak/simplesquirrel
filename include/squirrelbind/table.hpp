#pragma once
#ifndef SQUIRREL_BIND_TABLE_HEADER_H
#define SQUIRREL_BIND_TABLE_HEADER_H

#include "class.hpp"

namespace SquirrelBind {
    /**
    * @brief Squirrel table object
    */
    class SqTable: public SqObject {
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
        explicit SqTable(const SqTable& other);
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
        * @throws SqRuntimeException if VM is invalid
        * @throws SqNotFoundException if function was not found
        * @throws SqTypeException if the object found is not a function
        * @returns SqClass object references the found class
        */
        SqClass findClass(const char* name) const;
        /**
        * @brief Adds a new class type to this table
        * @throws SqRuntimeException if VM is invalid
        * @returns SqClass object references the added class
        */
        template<typename T, typename... Args>
        SqClass addClass(const char* name, const std::function<T*(Args...)>& allocator = std::bind(&detail::defaultClassAllocator<T>)){
			if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
            sq_pushobject(vm, obj);
            SqClass cls(detail::addClass(vm, name, allocator));
            sq_pop(vm, 1);
            return cls;
        }
		/**
        * @brief Adds a new class type to this table
        * @throws SqRuntimeException if VM is invalid
        * @returns SqClass object references the added class
        */
        template<typename T, typename... Args>
        SqClass addClass(const char* name, const SqClass::Ctor<T(Args...)>& constructor){
            const std::function<T*(Args...)> func = &constructor.allocate;
            return addClass<T>(name, func);
        }
		/**
        * @brief Adds a new function type to this table
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object references the added function
        */
		template<typename R, typename... Args>
		SqFunction addFunc(const char* name, const std::function<R(Args...)>& func){
			if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
			SqFunction ret(vm);
            sq_pushobject(vm, obj);
            detail::addFunc(vm, name, func);
            sq_pop(vm, 1);
            return ret;
        }
		/**
        * @brief Adds a new lambda type to this table
        * @throws SqRuntimeException if VM is invalid
        * @returns SqFunction object that references the added function
        */
		template<typename F>
		SqFunction addFunc(const char* name, const F& lambda) {
			return addFunc(name, detail::make_function(lambda));
		}
        /**
        * @brief Copy assingment operator
        */
        SqTable& operator = (const SqTable& other);
        /**
        * @brief Move assingment operator
        */
        SqTable& operator = (SqTable&& other) NOEXCEPT;
    };
}

#endif