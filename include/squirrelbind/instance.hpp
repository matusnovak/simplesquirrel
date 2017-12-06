#pragma once
#ifndef SQUIRREL_BIND_INSTANCE_HEADER_H
#define SQUIRREL_BIND_INSTANCE_HEADER_H

#include "object.hpp"

namespace SquirrelBind {
	class SqInstance: public SqObject {
	public:
		SqInstance(HSQUIRRELVM vm);

		SqInstance(const SqObject& object);

		SqInstance(const SqInstance& other);

		SqInstance(SqInstance&& other) NOEXCEPT;

		SqInstance& operator = (const SqInstance& other);

		SqInstance& operator = (SqInstance&& other) NOEXCEPT;
	};
}

#endif