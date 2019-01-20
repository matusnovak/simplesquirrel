#pragma once
#ifndef SSQ_FUNCTION_HEADER_H
#define SSQ_FUNCTION_HEADER_H

#include "object.hpp"
#include "exceptions.hpp"
#include "args.hpp"

namespace ssq {
    /**
    * @brief Squirrel function
    * @ingroup simplesquirrel
    */
    class SSQ_API Function: public Object {
    public:
        /**
        * @brief Constructs empty function object
        */
        explicit Function(HSQUIRRELVM vm);
        /**
        * @brief Destructor
        */
        virtual ~Function() = default;
        /**
        * @brief Converts Object to Function
        * @throws TypeException if the Object is not type of a function
        */
        explicit Function(const Object& object);
        /**
        * @brief Copy constructor
        */
        Function(const Function& other);
        /**
        * @brief Move constructor
        */
        Function(Function&& other) NOEXCEPT;
        /**
        * @brief Returns the number of parameters needed by the function
        * @note This ignores the "this" pointer
        */
        unsigned int getNumOfParams() const;
        /**
        * @brief Copy assingment operator
        */
        Function& operator = (const Function& other);
        /**
        * @brief Move assingment operator
        */
        Function& operator = (Function&& other) NOEXCEPT;
    };
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline Function popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_CLOSURE);
            Function val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Table from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }
    }
#endif
}

#endif