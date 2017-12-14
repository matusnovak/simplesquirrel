#pragma once
#ifndef SQUIRREL_BIND_FUNCTION_HEADER_H
#define SQUIRREL_BIND_FUNCTION_HEADER_H

#include "object.hpp"
#include "exceptions.hpp"
#include "args.hpp"

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
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline SqFunction popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_CLOSURE);
            SqFunction val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw SqTypeException("Could not get SqTable from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif