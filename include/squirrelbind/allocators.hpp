#pragma once
#ifndef SQUIRREL_BIND_ALLOCATORS_HEADER_H
#define SQUIRREL_BIND_ALLOCATORS_HEADER_H

#include "object.hpp"
#include <functional>

namespace SquirrelBind {
	namespace detail {
		template<class Ret>
		struct FuncPtr {
			const std::function<Ret()>* ptr;
		};

		template<class Ret, typename... Args>
		struct FuncPtr<Ret(Args...)> {
			const std::function<Ret(Args...)>* ptr;
		};

		template<class T>
		static T* defaultClassAllocator() {
			return new T();
        }

		template<class T>
		static SQInteger classDestructor(SQUserPointer ptr, SQInteger size) {
			T *p = static_cast<T*>(ptr);
			delete p;
			return 0;
		}

		template<class T, class... Args>
		static SQInteger classAllocator(HSQUIRRELVM vm) {
			static const std::size_t nparams = sizeof...(Args);
			int off = nparams;

			FuncPtr<T*(Args...)>* funcPtr;
			sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);
			int index = nparams + 1;

			T* p = funcPtr->ptr->operator()(pop<Args>::f(vm, index--)...);
			sq_setinstanceup(vm, -2 -off, p);
			sq_setreleasehook(vm, -2 -off, &detail::classDestructor<T>);

			sq_getclass(vm, -2 -off);
			sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(typeid(T).hash_code()));
			sq_pop(vm, 1); // Pop class
			return nparams;
		}
	}
}

#endif