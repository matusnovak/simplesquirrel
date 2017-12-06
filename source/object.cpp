#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include <squirrel.h>
#include <algorithm>
#include <iostream>

namespace SquirrelBind {

	const char* sqTypeToStr(SqType type) {
		switch (type) {
		case SqType::NULLPTR: return "NULLPTR";
		case SqType::INTEGER: return "INTEGER";
		case SqType::FLOAT: return "FLOAT";
		case SqType::BOOL: return "BOOL";
		case SqType::STRING: return "STRING";
		case SqType::TABLE: return "TABLE";
		case SqType::ARRAY: return "ARRAY";
		case SqType::USERDATA: return "USERDATA";
		case SqType::CLOSURE: return "CLOSURE";
		case SqType::NATIVECLOSURE: return "NATIVECLOSURE";
		case SqType::GENERATOR: return "GENERATOR";
		case SqType::USERPOINTER: return "USERPOINTER";
		case SqType::THREAD: return "THREAD";
		case SqType::FUNCPROTO: return "FUNCPROTO";
		case SqType::CLASS: return "CLASS";
		case SqType::INSTANCE: return "INSTANCE";
		case SqType::WEAKREF: return "WEAKREF";
		case SqType::OUTER: return "OUTER";
		default: return "UNKNOWN";
		}
	}

	SqObject::SqObject() :vm(nullptr) {
		sq_resetobject(&obj);
	}

	SqObject::SqObject(HSQUIRRELVM vm):vm(vm) {
		sq_resetobject(&obj);
	}

	SqObject::~SqObject() {
		if (vm != nullptr && !sq_isnull(obj)) {
			sq_release(vm, &obj);
			sq_resetobject(&obj);
		}
	}

	void SqObject::swap(SqObject& other) NOEXCEPT {
		using std::swap;
		swap(obj, other.obj);
		swap(vm, other.vm);
	}

	SqObject::SqObject(const SqObject& other) :vm(other.vm), obj(other.obj) {
		if (vm != nullptr && !other.isEmpty()) {
			sq_addref(vm, &obj);
		}
	}

	SqObject::SqObject(SqObject&& other) NOEXCEPT :vm(nullptr) {
		vm = nullptr;
		sq_resetobject(&obj);
		swap(other);
	}

	bool SqObject::isEmpty() const {
		return sq_isnull(obj);
	}

	const HSQOBJECT& SqObject::get() const {
		return obj;
	}

	HSQOBJECT& SqObject::get() {
		return obj;
	}

	SqObject SqObject::find(const char* name) const {
		if (vm == nullptr) throw SqRuntimeException("VM is not initialised");

		SqObject ret(vm);

		sq_pushobject(vm, obj);
		sq_pushstring(vm, name, strlen(name));

		if (SQ_FAILED(sq_get(vm, -2))) {
			sq_pop(vm, 1);
			throw SqNotFoundException("Not found");
		}

		sq_getstackobj(vm, -1, &ret.get());
		sq_addref(vm, &ret.get());
		sq_pop(vm, 2);

		return ret;
	}

	SqType SqObject::getType() const {
		if (isEmpty()) return SqType::NULLPTR;

		/*sq_pushobject(vm, obj);
		auto valueType = sq_gettype(vm, -1);
		sq_pop(vm, 1);*/

		return SqType(obj._type);
	}

	const char* SqObject::getTypeStr() const {
		return sqTypeToStr(getType());
	}

	/**
	* @brief Returns the Squirrel virtual machine handle associated
	* with this instance
	*/
	const HSQUIRRELVM& SqObject::getHandle() const {
		return vm;
	}

	SqObject& SqObject::operator = (const SqObject& other) {
		if (this != &other) {
			SqObject o(other);
			swap(o);
		}
		return *this;
	}

	SqObject& SqObject::operator = (SqObject&& other) NOEXCEPT {
		if (this != &other) {
			swap(other);
		}
		return *this;
	}

	int SqObject::asInt() const {
		if (getType() != SqType::INTEGER) throw SqTypeException("Bad cast", "INTEGER", getTypeStr());
		SQInteger val;
		sq_pushobject(vm, obj);
		sq_getinteger(vm, -1, &val);
		sq_pop(vm, 1);
		return static_cast<int>(val);
	}

	float SqObject::asFloat() const {
		if (getType() != SqType::FLOAT) throw SqTypeException("Bad cast", "FLOAT", getTypeStr());
		SQFloat val;
		sq_pushobject(vm, obj);
		sq_getfloat(vm, -1, &val);
		sq_pop(vm, 1);
		return static_cast<float>(val);
	}

	std::string SqObject::asString() const {
		if (getType() != SqType::STRING) throw SqTypeException("Bad cast", "STRING", getTypeStr());
		const SQChar* val;
		sq_pushobject(vm, obj);
		sq_getstring(vm, -1, &val);
		sq_pop(vm, 1);
		return std::string(val == nullptr ? "" : val);
	}

	bool SqObject::asBool() const {
		if (getType() != SqType::BOOL) throw SqTypeException("Bad cast", "BOOL", getTypeStr());
		SQBool val;
		sq_pushobject(vm, obj);
		sq_getbool(vm, -1, &val);
		sq_pop(vm, 1);
		return static_cast<bool>(val);
	}

	SqFunction SqObject::asFunction() const {
		auto type = getType();
		if (type != SqType::CLOSURE && type != SqType::NATIVECLOSURE) throw SqTypeException("Bad cast", "CLOSURE", getTypeStr());
		return SqFunction(*this);
	}

	SqInstance SqObject::asInstance() const {
		auto type = getType();
		if (type != SqType::INSTANCE) throw SqTypeException("Bad cast", "INSTANCE", getTypeStr());
		return SqInstance(*this);
	}
	
	SqClass SqObject::asClass() const {
		auto type = getType();
		if (type != SqType::CLASS) throw SqTypeException("Bad cast", "CLASS", getTypeStr());
		return SqClass(*this);
	}
}
