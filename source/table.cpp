#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/table.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
	SqTable::SqTable():SqObject() {
			
	}

	SqTable::SqTable(HSQUIRRELVM vm):SqObject(vm) {
			
	}

	SqTable::SqTable(const SqTable& other):SqObject(other) {
			
	}

	SqTable::SqTable(SqTable&& other) NOEXCEPT :SqObject(std::forward<SqTable>(other)) {
			
	}

	SqFunction SqTable::findFunc(const char* name) const {
		SqObject object = SqObject::find(name);
		return SqFunction(object);
	}

	SqClass SqTable::findClass(const char* name) const {
		SqObject object = SqObject::find(name);
		return SqClass(object);
	}

	SqTable& SqTable::operator = (const SqTable& other){
		SqObject::operator = (other);
		return *this;
	}

	SqTable& SqTable::operator = (SqTable&& other) NOEXCEPT {
		SqObject::operator = (std::forward<SqTable>(other));
		return *this;
	}
}
