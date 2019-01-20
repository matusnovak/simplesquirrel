#pragma once
#ifndef SSQ_ENUM_HEADER_H
#define SSQ_ENUM_HEADER_H

#include "class.hpp"

namespace ssq {
    /**
    * @brief Squirrel table object
    * @ingroup simplesquirrel
    */
    class SSQ_API Enum: public Object {
    public:
        /**
        * @brief Creates empty table with null VM
        * @note This object will be unusable
        */
        Enum();
        /**
        * @brief Destructor
        */
        virtual ~Enum() = default;
        /**
        * @brief Converts Object to Enum
        * @throws TypeException if the Object is not type of an enum (table)
        */
        explicit Enum(const Object& other);
        /**
        * @brief Creates empty enum
        */
        explicit Enum(HSQUIRRELVM vm);
        /**
        * @brief Copy constructor
        */
        Enum(const Enum& other);
        /**
        * @brief Move constructor
        */
        Enum(Enum&& other) NOEXCEPT;
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
        Enum& operator = (const Enum& other);
        /**
        * @brief Move assingment operator
        */
        Enum& operator = (Enum&& other) NOEXCEPT;
    };
}

#endif