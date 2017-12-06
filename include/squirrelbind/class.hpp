#pragma once
#ifndef SQUIRREL_BIND_CLASS_HEADER_H
#define SQUIRREL_BIND_CLASS_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
	class SqClass: public SqObject {
	public:
		template<class Signature>
		struct Ctor;

		template<class T, class... Args>
		struct Ctor<T(Args...)> {
			static T* allocate(Args&&... args) {
				return new T(std::forward<Args>(args)...);
			}
		};

		SqClass(HSQUIRRELVM vm);

		SqClass(const SqObject& object);

		SqClass(const SqClass& other);

		SqClass(SqClass&& other) NOEXCEPT;

		SqFunction findFunc(const char* name) const;

		SqClass& operator = (const SqClass& other);

		SqClass& operator = (SqClass&& other) NOEXCEPT;
	};
}

#endif