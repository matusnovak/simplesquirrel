#include "../include/squirrelbind/object.hpp"
#include "../include/squirrelbind/function.hpp"
#include "../include/squirrelbind/class.hpp"
#include "../include/squirrelbind/instance.hpp"
#include "../include/squirrelbind/exceptions.hpp"
#include "../include/squirrelbind/table.hpp"
#include "../include/squirrelbind/array.hpp"
#include <squirrel.h>
#include <cstring>

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

    SqObject::SqObject(HSQUIRRELVM vm) : vm(vm) {
        if (vm == nullptr) throw SqRuntimeException("VM is not initialised");
        sq_resetobject(&obj);
    }

    SqObject::~SqObject() {
        if (vm != nullptr && !sq_isnull(obj)) {
            sq_release(vm, &obj);
            sq_resetobject(&obj);
        }
    }

    void SqObject::reset() {
        if (vm != nullptr && !sq_isnull(obj)) {
            sq_release(vm, &obj);
        }
        sq_resetobject(&obj);
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

    const HSQOBJECT& SqObject::getRaw() const {
        return obj;
    }

    HSQOBJECT& SqObject::getRaw() {
        return obj;
    }

    bool SqObject::isNull() const {
        return getType() == SqType::NULLPTR;
    }

    SqObject SqObject::find(const char* name) const {
        if (vm == nullptr) throw SqRuntimeException("VM is not initialised");

        SqObject ret(vm);

        sq_pushobject(vm, obj);
        sq_pushstring(vm, name, strlen(name));

        if (SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            throw SqNotFoundException(name);
        }

        sq_getstackobj(vm, -1, &ret.getRaw());
        sq_addref(vm, &ret.getRaw());
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

    size_t SqObject::getTypeTag() const {
        if (isEmpty()) return 0;
        SQUserPointer typetag;
        sq_pushobject(vm, obj);
        sq_gettypetag(vm, -1, &typetag);
        sq_pop(vm, 1);
        return reinterpret_cast<size_t>(typetag);
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

#ifdef _SQ64
    int64_t SqObject::toInt() const {
        return to<int64_t>();
    }
#else
    int32_t SqObject::toInt() const {
        return to<int32_t>();
    }
#endif

#ifdef SQUSEDOUBLE
    double SqObject::toFloat() const {
        return to<double>();
    }
#else
    float SqObject::toFloat() const {
        return to<float>();
    }
#endif

#ifdef SQUNICODE
    std::wstring SqObject::toString() const {
        return to<std::wstring>();
    }
#else
    std::string SqObject::toString() const {
        return to<std::string>();
    }
#endif

    bool SqObject::toBool() const {
        return to<bool>();
    }

    SqFunction SqObject::toFunction() const {
        auto type = getType();
        return SqFunction(*this);
    }

    SqInstance SqObject::toInstance() const {
        auto type = getType();
        return SqInstance(*this);
    }
    
    SqClass SqObject::toClass() const {
        auto type = getType();
        return SqClass(*this);
    }

    SqTable SqObject::toTable() const {
        auto type = getType();
        return SqTable(*this);
    }

    SqArray SqObject::toArray() const {
        auto type = getType();
        return SqArray(*this);
    }
}
