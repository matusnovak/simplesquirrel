#pragma once
#ifndef SSQ_INSTANCE_HEADER_H
#define SSQ_INSTANCE_HEADER_H

#include "object.hpp"
#include "args.hpp"

namespace ssq {
    class Class;
    /**
    * @brief Squirrel intance of class object
    * @ingroup simplesquirrel
    */
    class SSQ_API Instance: public Object {
    public:
        /**
        * @brief Constructs empty invalid instance
        */
        Instance();
        /**
        * @brief Destructor
        */
        virtual ~Instance() = default;
        /**
        * @brief Constructs empty instance
        */
        Instance(HSQUIRRELVM vm);
        /**
        * @brief Converts Object to Instance
        * @throws TypeException if the Object is not type of an instance
        */
        explicit Instance(const Object& object);
        /**
        * @brief Copy constructor
        */
        Instance(const Instance& other);
        /**
        * @brief Move constructor
        */
        Instance(Instance&& other) NOEXCEPT;
        /**
        * @brief Returns the class associated with this instance
        * @throws TypeException if something went wrong
        */
        Class getClass();
        /**
        * @brief Copy assingment operator
        */ 
        Instance& operator = (const Instance& other);
        /**
        * @brief Move assingment operator
        */
        Instance& operator = (Instance&& other) NOEXCEPT;
    };

    /**
     * @brief Weak reference class that does not extend the life of the instance
     * @ingroup simplesquirrel
     */
    class SSQ_API SqWeakRef: public Instance {
    public:
        SqWeakRef();
        SqWeakRef(HSQUIRRELVM vm);
        SqWeakRef(const SqWeakRef& other);
        SqWeakRef(SqWeakRef&& other);
        explicit SqWeakRef(const Instance& instance);

        void swap(SqWeakRef& other);

        SqWeakRef& operator = (const SqWeakRef& other);
        SqWeakRef& operator = (SqWeakRef&& other);
    };

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template<>
        inline Instance popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INSTANCE);
            Instance val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Instance from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }

        template<>
        inline SqWeakRef popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INSTANCE);
            SqWeakRef val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Instance from squirrel stack");
            return val;
        }
    }
#endif
}

#endif