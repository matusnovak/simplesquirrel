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

namespace SquirrelBind {
	class SqFunction;
	class SqTable;
	class SqClass;
	class SqInstance;

	typedef std::unordered_map<size_t, HSQOBJECT> ClassMap;

	enum class SqType {
		NULLPTR = OT_NULL,
		INTEGER = OT_INTEGER,
		FLOAT = OT_FLOAT,
		BOOL = OT_BOOL,
		STRING = OT_STRING,
		TABLE = OT_TABLE,
		ARRAY = OT_ARRAY,
		USERDATA = OT_USERDATA,
		CLOSURE = OT_CLOSURE,
		NATIVECLOSURE = OT_NATIVECLOSURE,
		GENERATOR = OT_GENERATOR,
		USERPOINTER = OT_USERPOINTER,
		THREAD = OT_THREAD,
		FUNCPROTO = OT_FUNCPROTO,
		CLASS = OT_CLASS,
		INSTANCE = OT_INSTANCE,
		WEAKREF = OT_WEAKREF,
		OUTER = OT_OUTER
	};

	const char* sqTypeToStr(SqType type);

	namespace detail {
		template<class T>
		static HSQOBJECT& getClassObjectOrCreate(HSQUIRRELVM vm);
	}

	class SqObject {
	public:
		SqObject();
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

		int asInt() const;
		float asFloat() const;
		std::string asString() const;
		bool asBool() const;
		SqFunction asFunction() const;
		SqInstance asInstance() const;
		SqClass asClass() const;

		SqObject& operator = (const SqObject& other);
		SqObject& operator = (SqObject&& other) NOEXCEPT;

	protected:
		HSQUIRRELVM vm;
		HSQOBJECT obj;
	};
}

#endif