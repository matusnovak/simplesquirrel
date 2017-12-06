#pragma once
#ifndef SQUIRREL_BIND_CLASS_HEADER_H
#define SQUIRREL_BIND_CLASS_HEADER_H

#include "table.hpp"

namespace SquirrelBind {
	class SqClass: public SqTable {
	public:
		SqClass(HSQUIRRELVM vm);

		SqClass(const SqObject& object);

		SqClass(const SqClass& other);

		SqClass(SqClass&& other) NOEXCEPT;

		SqClass& operator = (const SqClass& other);

		SqClass& operator = (SqClass&& other) NOEXCEPT;
	};
}

#endif