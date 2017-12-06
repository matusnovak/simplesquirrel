#pragma once
#ifndef SQUIRREL_BIND_TABLE_HEADER_H
#define SQUIRREL_BIND_TABLE_HEADER_H

namespace SquirrelBind {
	class SqClass;

	class SqTable: public SqObject {
	public:
		SqTable();

		SqTable(HSQUIRRELVM vm);

		SqTable(const SqTable& other);

		SqTable(SqTable&& other) NOEXCEPT;

		SqFunction findFunc(const char* name) const;

		SqClass findClass(const char* name) const;

		SqTable& operator = (const SqTable& other);

		SqTable& operator = (SqTable&& other) NOEXCEPT;
	};
}

#endif