#pragma once
#ifndef SSQ_ALLOCATORS_HEADER_H
#define SSQ_ALLOCATORS_HEADER_H

#include "object.hpp"
#include "args.hpp"
#include <functional>

namespace ssq {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        template <size_t... Is>
        struct index_list {
        };

        // Declare primary template for index range builder
        template <size_t MIN, size_t N, size_t... Is>
        struct range_builder;

        // Base step
        template <size_t MIN, size_t... Is>
        struct range_builder<MIN, MIN, Is...> {
            typedef index_list<Is...> type;
        };

        // Induction step
        template <size_t MIN, size_t N, size_t... Is>
        struct range_builder : public range_builder<MIN, N - 1, N - 1, Is...> {
        };

        // Meta-function that returns a [MIN, MAX) index range
        template<size_t MIN, size_t MAX>
        using index_range = typename detail::range_builder<MIN, MAX>::type;

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

        template<class T, class... Args, size_t... Is>
        static T* callConstructor(HSQUIRRELVM vm, FuncPtr<T*(Args...)>* funcPtr, index_list<Is...>) {
            return funcPtr->ptr->operator()(detail::pop<Args>(vm, Is + 2)...);
        }

        template<class T, class... Args>
        static SQInteger classAllocator(HSQUIRRELVM vm) {
            static const std::size_t nparams = sizeof...(Args);
            int off = nparams;

            FuncPtr<T*(Args...)>* funcPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);

            T* p = callConstructor<T, Args...>(vm, funcPtr, index_range<0, sizeof...(Args)>());
            sq_setinstanceup(vm, -2 -off, p);
            sq_setreleasehook(vm, -2 -off, &detail::classDestructor<T>);

            sq_getclass(vm, -2 -off);
            sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(typeid(T*).hash_code()));
            sq_pop(vm, 1); // Pop class
            return nparams;
        }

        template<class T, class... Args>
        static SQInteger classAllocatorNoRelease(HSQUIRRELVM vm) {
            static const std::size_t nparams = sizeof...(Args);
            int off = nparams;

            FuncPtr<T*(Args...)>* funcPtr;
            sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);

            T* p = callConstructor<T, Args...>(vm, funcPtr, index_range<0, sizeof...(Args)>());
            sq_setinstanceup(vm, -2 -off, p);

            sq_getclass(vm, -2 -off);
            sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(typeid(T*).hash_code()));
            sq_pop(vm, 1); // Pop class
            return nparams;
        }

        template<class Ret, class... Args>
        static SQInteger funcReleaseHook(SQUserPointer p, SQInteger size) {
            auto funcPtr = reinterpret_cast<FuncPtr<Ret(Args...)>*>(p);
            delete funcPtr->ptr;
            return 0;
        }
    }

    /**
     * @ingroup simplesquirrel
     */
    template<typename T>
    inline T Object::to() const {
        sq_pushobject(vm, obj);
        try {
            auto ret = detail::pop<T>(vm, -1);
            sq_pop(vm, 1);
            return ret;
        } catch (...) {
            sq_pop(vm, 1);
            std::rethrow_exception(std::current_exception());
        }
    }
#endif
}

#endif