#pragma once
#ifndef SQUIRREL_BIND_ARGS_HEADER_H
#define SQUIRREL_BIND_ARGS_HEADER_H

#include "exceptions.hpp"
#include <squirrel.h>
#include <iostream>

namespace SquirrelBind {
	namespace detail {
		template<class T>
		struct push {
			static void f(HSQUIRRELVM vm, T value);
		};
		
		template<class T>
		inline void push<T>::f(HSQUIRRELVM vm, T value) {
			throw SqTypeException("Can't push this type to VM", "NOT_IMPLEMENTED_TYPE", typeid(T).name());
		}

		inline void push<int>::f(HSQUIRRELVM vm, int value) {
			sq_pushinteger(vm, value);
		}

		inline void push<float>::f(HSQUIRRELVM vm, float value) {
			sq_pushfloat(vm, value);
		}

		inline void push<short>::f(HSQUIRRELVM vm, short value) {
			sq_pushinteger(vm, value);
		}

		inline void push<char>::f(HSQUIRRELVM vm, char value) {
			sq_pushinteger(vm, value);
		}

		inline void push<long>::f(HSQUIRRELVM vm, long value) {
			sq_pushinteger(vm, value);
		}

		inline void push<long long>::f(HSQUIRRELVM vm, long long value) { // TODO
			sq_pushinteger(vm, static_cast<SQInteger>(value));
		}

		inline void push<bool>::f(HSQUIRRELVM vm, bool value) {
			sq_pushbool(vm, value);
		}
	}
}

#endif