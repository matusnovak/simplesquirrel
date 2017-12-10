#pragma once
#ifndef SQUIRREL_BIND_FUNCTION_HEADER_H
#define SQUIRREL_BIND_FUNCTION_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
    /**
    * @brief Squirrel function
    */
    class SQBIND_API SqFunction: public SqObject {
    public:
        /**
        * @brief Constructs empty function object
        */
        explicit SqFunction(HSQUIRRELVM vm);
        /**
        * @brief Converts SqObject to SqFunction
        * @throws SqTypeException if the SqObject is not type of a function
        */
        explicit SqFunction(const SqObject& object);
        /**
        * @brief Copy constructor
        */
        SqFunction(const SqFunction& other);
        /**
        * @brief Move constructor
        */
        SqFunction(SqFunction&& other) NOEXCEPT;
        /**
        * @brief Returns the number of parameters needed by the function
        * @note This ignores the "this" pointer
        */
        unsigned int getNumOfParams() const;
        /**
        * @brief Copy assingment operator
        */
        SqFunction& operator = (const SqFunction& other);
        /**
        * @brief Move assingment operator
        */
        SqFunction& operator = (SqFunction&& other) NOEXCEPT;
    };
}

#endif