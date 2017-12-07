#pragma once
#ifndef SQUIRREL_BIND_ARGS_HEADER_H
#define SQUIRREL_BIND_ARGS_HEADER_H

#include "exceptions.hpp"
#include <squirrel.h>
#include <iostream>
#include <typeinfo>

namespace SquirrelBind {
    namespace detail {
        inline void checkType(HSQUIRRELVM vm, SQInteger index, SQObjectType expected){
            auto type = sq_gettype(vm, index);
            if (type != expected) throw SqTypeException("bad cast", sqTypeToStr(SqType(expected)), sqTypeToStr(SqType(type)));
        }

        template<typename T>
        inline T popValue(HSQUIRRELVM vm, SQInteger index){
            throw SqTypeException("Cannot pop value, type not supported!", "UNSUPPROTED", typeid(T).name());
        }

		template<typename T>
		inline T popPointer(HSQUIRRELVM vm, SQInteger index) {
	        checkType(vm, index, OT_INSTANCE);
			SQUserPointer val;
			sq_gettypetag(vm, index, &val);
			if(reinterpret_cast<size_t>(val) != typeid(T).hash_code()) {
				throw SqTypeException("bad cast", typeid(T).name(), "UNKNOWN");
			}
			if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
				throw SqTypeException("Could not get instance from squirrel stack");
			}
			return reinterpret_cast<T>(val);
        }

		template<>
        inline char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get char from squirrel stack");
            return static_cast<char>(val);
        }

		template<>
        inline signed char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get signed char from squirrel stack");
            return static_cast<signed char>(val);
        }

        template<>
        inline short popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get short from squirrel stack");
            return static_cast<short>(val);
        }

        template<>
        inline int popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get int from squirrel stack");
            return static_cast<int>(val);
        }

        template<>
        inline long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get long from squirrel stack");
            return static_cast<long>(val);
        }

		template<>
        inline unsigned char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get unsigned char from squirrel stack");
            return static_cast<unsigned char>(val);
        }

        template<>
        inline unsigned short popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get unsigned short from squirrel stack");
            return static_cast<unsigned short>(val);
        }

        template<>
        inline unsigned int popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get unsigned int from squirrel stack");
            return static_cast<unsigned int>(val);
        }

        template<>
        inline unsigned long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get unsigned long from squirrel stack");
            return static_cast<unsigned long>(val);
        }

#ifdef _SQ64
		template<>
        inline long long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get long long from squirrel stack");
            return static_cast<long long>(val);
        }

		template<>
        inline unsigned long long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get unsigned long long from squirrel stack");
            return static_cast<unsigned long long>(val);
        }
#endif

#ifdef SQUSEDOUBLE
		template<>
        inline double popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQFloat val;
            if (SQ_FAILED(sq_getfloat(vm, index, &val))) throw SqTypeException("Could not get double from squirrel stack");
            return static_cast<double>(val);
        }
#endif

        template<>
        inline float popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_FLOAT);
            SQFloat val;
            if (SQ_FAILED(sq_getfloat(vm, index, &val))) throw SqTypeException("Could not get float from squirrel stack");
            return static_cast<float>(val);
        }

        template<>
        inline bool popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_BOOL);
            SQBool val;
            if (SQ_FAILED(sq_getbool(vm, index, &val))) throw SqTypeException("Could not get bool from squirrel stack");
            return static_cast<bool>(val);
        }

#ifdef SQUNICODE
		template<>
        inline std::wstring popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_STRING);
            const SQChar* val;
            if (SQ_FAILED(sq_getstring(vm, index, &val))) throw SqTypeException("Could not get string from squirrel stack");
            return std::wstring(val == nullptr ? L"" : val);
        }
#else
		template<>
        inline std::string popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_STRING);
            const SQChar* val;
            if (SQ_FAILED(sq_getstring(vm, index, &val))) throw SqTypeException("Could not get string from squirrel stack");
            return std::string(val == nullptr ? "" : val);
        }
#endif

        template <typename T> inline typename std::enable_if<!std::is_pointer<T>::value, T>::type
        pop(HSQUIRRELVM vm, SQInteger index) { 
            return popValue<typename std::remove_cv<T>::type>(vm, index); 
        }

        template <typename T> inline typename std::enable_if<std::is_pointer<T>::value, T>::type
        pop(HSQUIRRELVM vm, SQInteger index) { 
            return popPointer<T>(vm, index);
        }

        /*template<typename T, typename std::enable_if<!std::is_pointer<T>::value>* = 0>
        T pop(HSQUIRRELVM vm, SQInteger index);

        template<typename T, typename std::enable_if<!std::is_pointer<T>::value>* = 0>
        T pop(HSQUIRRELVM vm, SQInteger index){
            return T();
        }

        template<typename std::enable_if<!std::is_pointer<int>::value>* = 0>
        int pop(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get int from squirrel stack");
            return static_cast<int>(val);
        }

        template<typename std::enable_if<!std::is_pointer<float>::value>* = 0>
        float pop(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_FLOAT);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw SqTypeException("Could not get int from squirrel stack");
            return static_cast<float>(val);
        }

        template<class T, typename std::enable_if<std::is_pointer<T>::value>* = 0>
        T pop(HSQUIRRELVM vm, SQInteger index){
            return nullptr;
        }*/

		template<typename T>
		struct pushByCopy {
			static void f(HSQUIRRELVM vm, const T& value) {
				throw SqTypeException("Cannot push value, type not supported!", "UNKNOWN", typeid(T).name());
			}
        };

		template<typename T>
        inline void pushValue(HSQUIRRELVM vm, const T& value){
			pushByCopy<T>::f(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const int& value) {
	        sq_pushinteger(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const short& value) {
	        sq_pushinteger(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const long& value) {
	        sq_pushinteger(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const char& value) {
	        sq_pushinteger(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const signed char& value) {
	        sq_pushinteger(vm, value);
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const unsigned int& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const int*>(&value));
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const unsigned short& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const short*>(&value));
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const unsigned long& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const long*>(&value));
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const unsigned char& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const char*>(&value));
        }

#ifdef _SQ64
		template<>
		inline void pushValue(HSQUIRRELVM vm, const unsigned long long& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const long long*>(&value));
        }

		template<>
		inline void pushValue(HSQUIRRELVM vm, const long long& value) {
	        sq_pushinteger(vm, *reinterpret_cast<const long long*>(&value));
        }
#endif

#ifdef SQUSEDOUBLE
		template<>
		inline void pushValue(HSQUIRRELVM vm, const double& value) {
	        sq_pushfloat(vm, value);
        }
#endif

		template<>
		inline void pushValue(HSQUIRRELVM vm, const float& value) {
	        sq_pushfloat(vm, value);
        }

#ifdef SQUNICODE
		template<>
		inline void pushValue(HSQUIRRELVM vm, const std::wstring& value) {
			sq_pushstring(vm, value.c_str(), value.size());
        }
#else
		template<>
		inline void pushValue(HSQUIRRELVM vm, const std::string& value) {
			sq_pushstring(vm, value.c_str(), value.size());
        }
#endif

		template <typename T, typename std::enable_if<!std::is_pointer<T>::value, T>::type* = nullptr>
        inline void push(HSQUIRRELVM vm, T value) { 
            pushValue<T>(vm, value); 
        }

        template <typename T, typename std::enable_if<std::is_pointer<T>::value, T>::type* = nullptr>
        inline void push(HSQUIRRELVM vm, T value) { 
			sq_pushnull(vm);
        }

        /*template<class T>
        struct push {
            static void f(HSQUIRRELVM vm, T value);
        };
        
        template<class T>
        inline void push<T>::f(HSQUIRRELVM vm, T value) {
            throw SqTypeException("Can't push this type to VM", "NOT_IMPLEMENTED_TYPE", typeid(T).name());
        }

        template<>
        inline void push<int>::f(HSQUIRRELVM vm, int value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void push<float>::f(HSQUIRRELVM vm, float value) {
            sq_pushfloat(vm, value);
        }

        template<>
        inline void push<short>::f(HSQUIRRELVM vm, short value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void push<char>::f(HSQUIRRELVM vm, char value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void push<long>::f(HSQUIRRELVM vm, long value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void push<long long>::f(HSQUIRRELVM vm, long long value) { // TODO
            sq_pushinteger(vm, static_cast<SQInteger>(value));
        }

        template<>
        inline void push<bool>::f(HSQUIRRELVM vm, bool value) {
            sq_pushbool(vm, value);
        }*/
    }
}

#endif