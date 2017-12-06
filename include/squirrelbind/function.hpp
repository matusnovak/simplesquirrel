#pragma once
#ifndef SQUIRREL_BIND_FUNCTION_HEADER_H
#define SQUIRREL_BIND_FUNCTION_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
	class SqFunction: public SqObject {
	public:
		SqFunction(HSQUIRRELVM vm);

		SqFunction(const SqObject& object);

		SqFunction(const SqFunction& other);

		SqFunction(SqFunction&& other) NOEXCEPT;

		unsigned int getNumOfParams() const;

		SqFunction& operator = (const SqFunction& other);

		SqFunction& operator = (SqFunction&& other) NOEXCEPT;
	};
}

#endif