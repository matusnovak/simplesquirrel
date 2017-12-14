#pragma once
#ifndef SQUIRREL_BIND_INSTANCE_HEADER_H
#define SQUIRREL_BIND_INSTANCE_HEADER_H

#include "object.hpp"
#include "args.hpp"

namespace SquirrelBind {
    class SqClass;
    /**
    * @brief Squirrel intance of class object
    */
    class SQBIND_API SqInstance: public SqObject {
    public:
        /**
        * @brief Constructs empty invalid instance
        */
        SqInstance();
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

    /**
     * @brief Weak reference class that does not extend the life of the instance
     */
    class SQBIND_API SqWeakRef: public SqInstance {
    public:
        SqWeakRef();
        SqWeakRef(HSQUIRRELVM vm);
        SqWeakRef(const SqWeakRef& other);
        SqWeakRef(SqWeakRef&& other);

        void swap(SqWeakRef& other);

        SqWeakRef& operator = (const SqWeakRef& other);
        SqWeakRef& operator = (SqWeakRef&& other);
    };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline SqInstance popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INSTANCE);
            SqInstance val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw SqTypeException("Could not get SqInstance from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }

        template<>
        inline SqWeakRef popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INSTANCE);
            SqWeakRef val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw SqTypeException("Could not get SqInstance from squirrel stack");
            return val;
        }
    }
#endif
}

#endif