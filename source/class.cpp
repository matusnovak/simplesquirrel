#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include "../include/squirrelbind/function.hpp"
#include <squirrel.h>
#include <forward_list>

namespace SquirrelBind {
	SqClass::SqClass(HSQUIRRELVM vm):SqObject(vm) {
			
	}

	SqClass::SqClass(const SqObject& object):SqObject(object.getHandle()) {
		if (vm != nullptr && !object.isEmpty()) {
			obj = object.get();
			sq_addref(vm, &obj);
		}
		if (object.getType() != SqType::CLASS) throw SqTypeException("Object is not a class", object.getTypeStr(), "CLASS");
	}

	SqClass::SqClass(const SqClass& other):SqObject(other) {
			
	}

	SqClass::SqClass(SqClass&& other) NOEXCEPT :SqObject(std::forward<SqClass>(other)) {
			
	}

	SqFunction SqClass::findFunc(const char* name) const {
		SqObject object = SqObject::find(name);
		return SqFunction(object);
	}

	SqClass& SqClass::operator = (const SqClass& other) {
		SqObject::operator = (other);
		return *this;
	}

	SqClass& SqClass::operator = (SqClass&& other) NOEXCEPT {
		SqObject::operator = (std::forward<SqClass>(other));
		return *this;
	}
}