#pragma once
#ifndef SQUIRREL_BIND_INSTANCE_HEADER_H
#define SQUIRREL_BIND_INSTANCE_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
    class SqClass;
    /**
    * @brief Squirrel intance of class object
    */
    class SQBIND_API SqInstance: public SqObject {
    public:
        /**
        * @brief Constructs empty instance
        */
        SqInstance(HSQUIRRELVM vm);
        /**
        * @brief Converts SqObject to SqInstance
        * @throws SqTypeException if the SqObject is not type of an instance
        */
        explicit SqInstance(const SqObject& object);
        /**
        * @brief Copy constructor
        */
        SqInstance(const SqInstance& other);
        /**
        * @brief Move constructor
        */
        SqInstance(SqInstance&& other) NOEXCEPT;
        /**
        * @brief Returns the class associated with this instance
        * @throws SqTypeException if something went wrong
        */
        SqClass getClass();
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