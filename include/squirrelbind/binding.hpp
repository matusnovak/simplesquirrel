#pragma once
#ifndef SQUIRREL_BIND_BINDING_HEADER_H
#define SQUIRREL_BIND_BINDING_HEADER_H

#include "allocators.hpp"
#include <functional>

namespace SquirrelBind {
	namespace detail {
		template <class A>
		static void paramPackerType(char* ptr) {
			*ptr = '.';
		}

		template <class ...B>
		static void paramPacker(char* ptr) {
			int _[] = { 0, (paramPackerType<B>(ptr++), 0)... };
			(void)_;
			*ptr = '\0';
		}

		template<class T, class... Args>
		static SqObject addClass(HSQUIRRELVM vm, const char* name, const std::function<T*(Args...)>& allocator) {
			static const std::size_t nparams = sizeof...(Args);

			SqObject clsObj(vm);
			
			sq_pushstring(vm, name, strlen(name));
			sq_newclass(vm, false);

			HSQOBJECT& obj = getClassObjectOrCreate<T>(vm);
			sq_getstackobj(vm, -1, &obj);
			sq_addref(vm, &obj);

			sq_getstackobj(vm, -1, &clsObj.get());
			sq_addref(vm, &clsObj.get());

			sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(typeid(T).hash_code()));

			sq_pushstring(vm, "constructor", -1);
			bindUserData<T*>(vm, allocator);
			static char params[33];
			paramPacker<T*, Args...>(params);

			sq_newclosure(vm, &detail::classAllocator<T, Args...>, 1);

			sq_setparamscheck(vm, nparams + 1, params);
			sq_newslot(vm, -3, false); // Add the constructor method

			sq_newslot(vm, -3, SQFalse); // Add the class

			return clsObj;
		}
	}
}

#endif