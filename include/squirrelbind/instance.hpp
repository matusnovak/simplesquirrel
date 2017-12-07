#pragma once
#ifndef SQUIRREL_BIND_INSTANCE_HEADER_H
#define SQUIRREL_BIND_INSTANCE_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
    /**
    * @brief Squirrel intance of class object
    */
    class SqInstance: public SqObject {
    public:
        /**
        * @brief Constructs empty instance
        */
        explicit SqInstance(HSQUIRRELVM vm);
        /**
        * @brief Converts SqObject to SqInstance
        * @throws SqTypeException if the SqObject is not type of an instance
        */
        explicit SqInstance(const SqObject& object);
        /**
        * @brief Copy constructor
        */
        explicit SqInstance(const SqInstance& other);
        /**
        * @brief Move constructor
        */
        SqInstance(SqInstance&& other) NOEXCEPT;
        /**
        * @brief Copy assingment operator
        */ 
        SqInstance& operator = (const SqInstance& other);
        /**
        * @brief Move assingment operator
        */
        SqInstance& operator = (SqInstance&& other) NOEXCEPT;
    };
}

#endif