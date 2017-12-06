#pragma once
#ifndef SQUIRREL_BIND_TABLE_HEADER_H
#define SQUIRREL_BIND_TABLE_HEADER_H
#include <functional>
#include "allocators.hpp"
#include "class.hpp"

namespace SquirrelBind {
	class SqTable: public SqObject {
	public:
		SqTable();

		SqTable(HSQUIRRELVM vm);

		SqTable(const SqTable& other);

		SqTable(SqTable&& other) NOEXCEPT;

		SqFunction findFunc(const char* name) const;

		SqClass findClass(const char* name) const;

		template<class T, class... Args>
        SqClass addClass(const std::string& name, const std::function<T*(Args...)>& allocator = std::bind(&detail::defaultClassAllocator<T>)){
			SqClass cls(vm);
			sq_pushobject(vm, obj);
            
            sq_pop(vm, 1);
			return cls;
        }

		template<class T, class... Args>
        SqClass addClass(const std::string& name, const SqClass::Ctor<T(Args...)>& constructor){
			const std::function<T*(Args...)> func = &constructor.allocate;
			return addClass<T>(name, func);
        }

		SqTable& operator = (const SqTable& other);

		SqTable& operator = (SqTable&& other) NOEXCEPT;
	};
}

#endif