#include "../include/simplesquirrel/object.hpp"
#include "../include/simplesquirrel/function.hpp"
#include "../include/simplesquirrel/class.hpp"
#include "../include/simplesquirrel/instance.hpp"
#include "../include/simplesquirrel/exceptions.hpp"
#include "../include/simplesquirrel/table.hpp"
#include "../include/simplesquirrel/array.hpp"
#include <squirrel.h>
#include <cstring>

namespace ssq {

    const char* typeToStr(Type type) {
        switch (type) {
        case Type::NULLPTR: return "NULLPTR";
        case Type::INTEGER: return "INTEGER";
        case Type::FLOAT: return "FLOAT";
        case Type::BOOL: return "BOOL";
        case Type::STRING: return "STRING";
        case Type::TABLE: return "TABLE";
        case Type::ARRAY: return "ARRAY";
        case Type::USERDATA: return "USERDATA";
        case Type::CLOSURE: return "CLOSURE";
        case Type::NATIVECLOSURE: return "NATIVECLOSURE";
        case Type::GENERATOR: return "GENERATOR";
        case Type::USERPOINTER: return "USERPOINTER";
        case Type::THREAD: return "THREAD";
        case Type::FUNCPROTO: return "FUNCPROTO";
        case Type::CLASS: return "CLASS";
        case Type::INSTANCE: return "INSTANCE";
        case Type::WEAKREF: return "WEAKREF";
        case Type::OUTER: return "OUTER";
        default: return "UNKNOWN";
        }
    }

    Object::Object() :vm(nullptr), weak(false) {
        sq_resetobject(&obj);
    }

    Object::Object(HSQUIRRELVM vm) : vm(vm), weak(false) {
        if (vm == nullptr) throw RuntimeException("VM is not initialised");
        sq_resetobject(&obj);
    }

    Object::~Object() {
        reset();
    }

    void Object::reset() {
        if (vm != nullptr && !sq_isnull(obj) && !weak) {
            sq_release(vm, &obj);
        }
        sq_resetobject(&obj);
        weak = false;
    }

    void Object::swap(Object& other) NOEXCEPT {
        using std::swap;
        swap(obj, other.obj);
        swap(vm, other.vm);
        swap(weak, other.weak);
    }

    Object::Object(const Object& other) :vm(other.vm), obj(other.obj), weak(other.weak) {
        if (vm != nullptr && !other.isEmpty() && !weak) {
            sq_addref(vm, &obj);
        }
    }

    Object::Object(Object&& other) NOEXCEPT :vm(nullptr) {
        vm = nullptr;
        sq_resetobject(&obj);
        swap(other);
    }

    bool Object::isEmpty() const {
        return sq_isnull(obj);
    }

    const HSQOBJECT& Object::getRaw() const {
        return obj;
    }

    HSQOBJECT& Object::getRaw() {
        return obj;
    }

    bool Object::isNull() const {
        return getType() == Type::NULLPTR;
    }

    Object Object::find(const char* name) const {
        if (vm == nullptr) throw RuntimeException("VM is not initialised");

        Object ret(vm);

        sq_pushobject(vm, obj);
        sq_pushstring(vm, name, strlen(name));

        if (SQ_FAILED(sq_get(vm, -2))) {
            sq_pop(vm, 1);
            throw NotFoundException(name);
        }

        sq_getstackobj(vm, -1, &ret.getRaw());
        sq_addref(vm, &ret.getRaw());
        sq_pop(vm, 2);

        return ret;
    }

    Type Object::getType() const {
        if (isEmpty()) return Type::NULLPTR;

        /*sq_pushobject(vm, obj);
        auto valueType = sq_gettype(vm, -1);
        sq_pop(vm, 1);*/

        return Type(obj._type);
    }

    size_t Object::getTypeTag() const {
        if (isEmpty()) return 0;
        SQUserPointer typetag;
        sq_pushobject(vm, obj);
        sq_gettypetag(vm, -1, &typetag);
        sq_pop(vm, 1);
        return reinterpret_cast<size_t>(typetag);
    }

    const char* Object::getTypeStr() const {
        return typeToStr(getType());
    }

    /**
    * @brief Returns the Squirrel virtual machine handle associated
    * with this instance
    */
    const HSQUIRRELVM& Object::getHandle() const {
        return vm;
    }

    Object& Object::operator = (const Object& other) {
        if (this != &other) {
            Object o(other);
            swap(o);
        }
        return *this;
    }

    Object& Object::operator = (Object&& other) NOEXCEPT {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

#ifdef _SQ64
    int64_t Object::toInt() const {
        return to<int64_t>();
    }
#else
    int32_t Object::toInt() const {
        return to<int32_t>();
    }
#endif

#ifdef SQUSEDOUBLE
    double Object::toFloat() const {
        return to<double>();
    }
#else
    float Object::toFloat() const {
        return to<float>();
    }
#endif

#ifdef SQUNICODE
    std::wstring Object::toString() const {
        return to<std::wstring>();
    }
#else
    std::string Object::toString() const {
        return to<std::string>();
    }
#endif

    bool Object::toBool() const {
        return to<bool>();
    }

    Function Object::toFunction() const {
        auto type = getType();
        return Function(*this);
    }

    Instance Object::toInstance() const {
        auto type = getType();
        return Instance(*this);
    }
    
    Class Object::toClass() const {
        auto type = getType();
        return Class(*this);
    }

    Table Object::toTable() const {
        auto type = getType();
        return Table(*this);
    }

    Array Object::toArray() const {
        auto type = getType();
        return Array(*this);
    }
}