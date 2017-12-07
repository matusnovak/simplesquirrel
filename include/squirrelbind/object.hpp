#pragma once
#ifndef SQUIRREL_BIND_OBJECT_HEADER_H
#define SQUIRREL_BIND_OBJECT_HEADER_H

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

#include "type.hpp"

namespace SquirrelBind {
    class SqFunction;
    class SqTable;
    class SqClass;
    class SqInstance;
    class SqTable;

	namespace detail {
		template <typename T> inline typename std::enable_if<!std::is_pointer<T>::value, T>::type
			pop(HSQUIRRELVM vm, SQInteger index);
	}

    /**
    * @brief Raw Squirrel object
    */
    class SqObject {
    public:
        /**
        * @brief Creates an empty object with null VM
        * @note This object won't be usable
        */
        SqObject();
        /**
        * @brief Creates an empty object
        */
        SqObject(HSQUIRRELVM vm);
        virtual ~SqObject();
        /**
        * @brief Swaps two objects
        */
        void swap(SqObject& other) NOEXCEPT;
        /**
        * @brief Copy constructor to copy the object reference
        */
        SqObject(const SqObject& other);
        /**
        * @brief Move constructor
        */
        SqObject(SqObject&& other) NOEXCEPT;
        /**
        * @brief Checks if the object is empty
        */
        bool isEmpty() const;
        /**
        * @brief Returns raw Squirrel object reference
        */
        const HSQOBJECT& get() const;
        /**
        * @brief Returns raw Squirrel object reference
        */
        HSQOBJECT& get();
        /**
        * @brief Finds object within this object
        */
        SqObject find(const char* name) const;
        /**
        * @brief Returns the type of the object
        */
        SqType getType() const;
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
        * @brief Returns the integer value of this object
        * @throws SqTypeException if this object is not an integer
        */
#ifdef _SQ64
        int64_t toInt() const;
#else
		int32_t toInt() const;
#endif
        /**
        * @brief Returns the float value of this object
        * @throws SqTypeException if this object is not a float
        */
#ifdef SQUSEDOUBLE
		double toFloat() const;
#else
        float toFloat() const;
#endif
        /**
        * @brief Returns the string value of this object
        * @throws SqTypeException if this object is not a sring
        */
#ifdef SQUNICODE
        std::wstring toString() const;
#else
		std::string toString() const;
#endif
        /**
        * @brief Returns the boolean value of this object
        * @throws SqTypeException if this object is not a boolean
        */
        bool toBool() const;
        /**
        * @brief Returns the SqFunction value of this object
        * @throws SqTypeException if this object is not a function
        */
        SqFunction toFunction() const;
        /**
        * @brief Returns the SqInstance value of this object
        * @throws SqTypeException if this object is not an instance
        */
        SqInstance toInstance() const;
        /**
        * @brief Returns the SqClass value of this object
        * @throws SqTypeException if this object is not a class
        */
        SqClass toClass() const;
        /**
        * @brief Returns the SqTable value of this object
        * @throws SqTypeException if this object is not a table
        */
        SqTable toTable() const;
		template<typename T>
		T to() const {
			sq_pushobject(vm, obj);
			auto ret = detail::pop<T>(vm, -1);
			sq_pop(vm, 1);
			return ret;
		}
        /**
        * @brief Copy assingment operator
        */
        SqObject& operator = (const SqObject& other);
        /**
        * @brief Move assingment operator
        */
        SqObject& operator = (SqObject&& other) NOEXCEPT;

    protected:
        HSQUIRRELVM vm;
        HSQOBJECT obj;
    };
}

#endif