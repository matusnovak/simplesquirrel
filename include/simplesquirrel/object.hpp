#pragma once
#ifndef SSQ_OBJECT_HEADER_H
#define SSQ_OBJECT_HEADER_H

#if !defined(HAS_NOEXCEPT) && !defined(NOEXCEPT)
    #if defined(__clang__)
        #if __has_feature(cxx_noexcept)
            #define HAS_NOEXCEPT
        #endif
    #else
        #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
            defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
            #define HAS_NOEXCEPT
        #endif
    #endif

    #ifdef HAS_NOEXCEPT
        #define NOEXCEPT noexcept
    #else
        #define NOEXCEPT
    #endif
#endif

#include <string>
#include <squirrel.h>
#include <unordered_map>
#include <stdint.h>
#include "exceptions.hpp"
#include "type.hpp"

namespace ssq {
    class Function;
    class Table;
    class Class;
    class Instance;
    class Table;
    class Array;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template <typename T> inline typename std::enable_if<!std::is_pointer<T>::value, T>::type
            pop(HSQUIRRELVM vm, SQInteger index);
    }
#endif

    /**
    * @brief Raw Squirrel object
    * @ingroup simplesquirrel
    */
    class SSQ_API Object {
    public:
        /**
        * @brief Creates an empty object with null VM
        * @note This object won't be usable
        */
        Object();
        /**
        * @brief Creates an empty object
        */
        Object(HSQUIRRELVM vm);
        virtual ~Object();
        /**
        * @brief Swaps two objects
        */
        void swap(Object& other) NOEXCEPT;
        /**
        * @brief Copy constructor to copy the object reference
        */
        Object(const Object& other);
        /**
        * @brief Move constructor
        */
        Object(Object&& other) NOEXCEPT;
        /**
        * @brief Checks if the object is empty
        */
        bool isEmpty() const;
        /**
        * @brief Returns raw Squirrel object reference
        */
        const HSQOBJECT& getRaw() const;
        /**
        * @brief Returns raw Squirrel object reference
        */
        HSQOBJECT& getRaw();
        /**
        * @brief Finds object within this object
        */
        Object find(const char* name) const;
        /**
        * @brief Returns the type of the object
        */
        Type getType() const;
        /**
        * @brief Returns the type of the object in string format
        */
        const char* getTypeStr() const;
        /**
        * @brief Returns the Squirrel virtual machine handle associated
        * with this instance
        */
        const HSQUIRRELVM& getHandle() const;
        /**
        * @brief Returns the typetag associated with this object
        * @note The typetag is equal to hash value of typeid(T)
        */
        size_t getTypeTag() const;
        /**
        * @brief Returns true if the object is nullptr
        */
        bool isNull() const;
        /**
        * @brief Releases the object and resets it to empty
        */
        void reset();
        /**
        * @brief Returns the integer value of this object
        * @throws TypeException if this object is not an integer
        */
#ifdef _SQ64
        int64_t toInt() const;
#else
        int32_t toInt() const;
#endif
        /**
        * @brief Returns the float value of this object
        * @throws TypeException if this object is not a float
        */
#ifdef SQUSEDOUBLE
        double toFloat() const;
#else
        float toFloat() const;
#endif
        /**
        * @brief Returns the string value of this object
        * @throws TypeException if this object is not a sring
        */
#ifdef SQUNICODE
        std::wstring toString() const;
#else
        std::string toString() const;
#endif
        /**
        * @brief Returns the boolean value of this object
        * @throws TypeException if this object is not a boolean
        */
        bool toBool() const;
        /**
        * @brief Returns the Function value of this object
        * @throws TypeException if this object is not a function
        */
        Function toFunction() const;
        /**
        * @brief Returns the Instance value of this object
        * @throws TypeException if this object is not an instance
        */
        Instance toInstance() const;
        /**
        * @brief Returns the Class value of this object
        * @throws TypeException if this object is not a class
        */
        Class toClass() const;
        /**
        * @brief Returns the Table value of this object
        * @throws TypeException if this object is not a table
        */
        Table toTable() const;
        /**
        * @brief Returns the Array value of this object
        * @throws TypeException if this object is not an array
        */
        Array toArray() const;
        /**
        * @brief Returns an arbitary value of this object
        * @throws TypeException if this object is not an type of T
        */
        template<typename T>
        T to() const;
        /**
        * @brief Unsafe cast this object into any pointer of type T
        * @throws TypeException if this object is not an instance
        */
        template<typename T>
        T toPtrUnsafe() const {
            if (getType() != Type::INSTANCE) {
                throw ssq::TypeException("bad cast", "INSTANCE", getTypeStr());
            }
            sq_pushobject(vm, obj);
            SQUserPointer val;
            sq_getinstanceup(vm, -1, &val, nullptr);
            sq_pop(vm, 1);
            return reinterpret_cast<T>(val);
        }
        /**
        * @brief Copy assingment operator
        */
        Object& operator = (const Object& other);
        /**
        * @brief Move assingment operator
        */
        Object& operator = (Object&& other) NOEXCEPT;

    protected:
        HSQUIRRELVM vm;
        HSQOBJECT obj;
        bool weak;
    };
}

#endif