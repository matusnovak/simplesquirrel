#pragma once
#ifndef SQUIRREL_BIND_ENUM_HEADER_H
#define SQUIRREL_BIND_ENUM_HEADER_H

#include "class.hpp"

namespace SquirrelBind {
    /**
    * @brief Squirrel table object
    */
    class SQBIND_API SqEnum: public SqObject {
    public:
        /**
        * @brief Creates empty table with null VM
        * @note This object will be unusable
        */
        SqEnum();
        /**
        * @brief Converts SqObject to SqEnum
        * @throws SqTypeException if the SqObject is not type of an enum (table)
        */
        explicit SqEnum(const SqObject& other);
        /**
        * @brief Creates empty enum
        */
        explicit SqEnum(HSQUIRRELVM vm);
        /**
        * @brief Copy constructor
        */
        SqEnum(const SqEnum& other);
        /**
        * @brief Move constructor
        */
        SqEnum(SqEnum&& other) NOEXCEPT;
        /**
         * @brief Adds a new key-value pair to this table
         */
        template<typename T>
        void addSlot(const char* name, const T& value) {
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name, strlen(name));
            detail::push<T>(vm, value);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
        }
        /**
        * @brief Copy assingment operator
        */
        SqEnum& operator = (const SqEnum& other);
        /**
        * @brief Move assingment operator
        */
        SqEnum& operator = (SqEnum&& other) NOEXCEPT;
    };
}

#endif