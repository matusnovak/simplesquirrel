#pragma once
#ifndef SSQ_ARGS_HEADER_H
#define SSQ_ARGS_HEADER_H

#include "exceptions.hpp"
#include <squirrel.h>
#include <iostream>
#include <typeinfo>
#include <vector>

namespace ssq {
    class Array;
    class Enum;
    class VM;
    class SqWeakRef;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    namespace detail {
        SSQ_API void addClassObj(HSQUIRRELVM vm, size_t hashCode, const HSQOBJECT& obj);
        SSQ_API const HSQOBJECT& getClassObj(HSQUIRRELVM vm, size_t hashCode);

        template<class T>
        static SQInteger classDestructor(SQUserPointer ptr, SQInteger size) {
            T* p = static_cast<T*>(ptr);
            delete p;
            return 0;
        }

        template<class T>
        static SQInteger classPtrDestructor(SQUserPointer ptr, SQInteger size) {
            T** p = static_cast<T**>(ptr);
            delete *p;
            return 0;
        }

        inline void checkType(HSQUIRRELVM vm, SQInteger index, SQObjectType expected){
            auto type = sq_gettype(vm, index);
            if (expected == OT_CLOSURE) {
                if (type != OT_CLOSURE && type != OT_NATIVECLOSURE) throw TypeException("bad cast", typeToStr(Type(expected)), typeToStr(Type(type)));
                return;
            }
            if (type != expected) throw TypeException("bad cast", typeToStr(Type(expected)), typeToStr(Type(type)));
        }

        template<typename T>
        inline T popValue(HSQUIRRELVM vm, SQInteger index){
            SQObjectType type = sq_gettype(vm, index);
            SQUserPointer ptr;
            SQUserPointer typetag;
            if(type == OT_USERDATA) {
                sq_getuserdata(vm, index, &ptr, &typetag);

                if(reinterpret_cast<size_t>(typetag) != typeid(T).hash_code()) {
                    throw TypeException("bad cast", typeid(T).name(), "UNKNOWN");
                }

                T** p = reinterpret_cast<T**>(ptr);
                return T(**p);
            } 
            else if(type == OT_INSTANCE) {
                sq_getinstanceup(vm, index, &ptr, &typetag);
                sq_gettypetag(vm, index, &typetag);

                if(reinterpret_cast<size_t>(typetag) != typeid(T*).hash_code()) {
                    throw TypeException("bad cast", typeid(T).name(), "UNKNOWN");
                }

                T* p = reinterpret_cast<T*>(ptr);
                return T(*p);
            }
            else {
                throw TypeException("bad cast", "INSTANCE", typeToStr(Type(type)));
            }
        }

        template<typename T>
        inline T popPointer(HSQUIRRELVM vm, SQInteger index) {
            auto type = sq_gettype(vm, index);
            if(type == OT_USERPOINTER) {
                SQUserPointer val;
                if (SQ_FAILED(sq_getuserpointer(vm, index, &val))) {
                    throw TypeException("Could not get instance from squirrel stack");
                }
                return reinterpret_cast<T>(val);
            }
            else {
                if (type != OT_INSTANCE) throw TypeException("bad cast", typeToStr(Type(OT_INSTANCE)), typeToStr(Type(type)));
                SQUserPointer val;
                /*sq_gettypetag(vm, index, &val);
                if (reinterpret_cast<size_t>(val) != typeid(T).hash_code()) {
                    throw TypeException("bad cast", typeid(T).name(), "UNKNOWN");
                }*/
                if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
                    throw TypeException("Could not get instance from squirrel stack");
                }
                return reinterpret_cast<T>(val);
            }
        }

        template<>
        inline Object popValue(HSQUIRRELVM vm, SQInteger index){
            Object val(vm);
            if (SQ_FAILED(sq_getstackobj(vm, index, &val.getRaw()))) throw TypeException("Could not get Object from squirrel stack");
            sq_addref(vm, &val.getRaw());
            return val;
        }

        template<>
        inline char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get char from squirrel stack");
            return static_cast<char>(val);
        }

        template<>
        inline signed char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get signed char from squirrel stack");
            return static_cast<signed char>(val);
        }

        template<>
        inline short popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get short from squirrel stack");
            return static_cast<short>(val);
        }

        template<>
        inline int popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get int from squirrel stack");
            return static_cast<int>(val);
        }

        template<>
        inline long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get long from squirrel stack");
            return static_cast<long>(val);
        }

        template<>
        inline unsigned char popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get unsigned char from squirrel stack");
            return static_cast<unsigned char>(val);
        }

        template<>
        inline unsigned short popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get unsigned short from squirrel stack");
            return static_cast<unsigned short>(val);
        }

        template<>
        inline unsigned int popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get unsigned int from squirrel stack");
            return static_cast<unsigned int>(val);
        }

        template<>
        inline unsigned long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get unsigned long from squirrel stack");
            return static_cast<unsigned long>(val);
        }

#ifdef _SQ64
        template<>
        inline long long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get long long from squirrel stack");
            return static_cast<long long>(val);
        }

        template<>
        inline unsigned long long popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQInteger val;
            if (SQ_FAILED(sq_getinteger(vm, index, &val))) throw TypeException("Could not get unsigned long long from squirrel stack");
            return static_cast<unsigned long long>(val);
        }
#endif

#ifdef SQUSEDOUBLE
        template<>
        inline double popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_INTEGER);
            SQFloat val;
            if (SQ_FAILED(sq_getfloat(vm, index, &val))) throw TypeException("Could not get double from squirrel stack");
            return static_cast<double>(val);
        }
#endif

        template<>
        inline float popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_FLOAT);
            SQFloat val;
            if (SQ_FAILED(sq_getfloat(vm, index, &val))) throw TypeException("Could not get float from squirrel stack");
            return static_cast<float>(val);
        }

        template<>
        inline bool popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_BOOL);
            SQBool val;
            if (SQ_FAILED(sq_getbool(vm, index, &val))) throw TypeException("Could not get bool from squirrel stack");
            return val == 1;
        }

#ifdef SQUNICODE
        template<>
        inline std::wstring popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_STRING);
            const SQChar* val;
            if (SQ_FAILED(sq_getstring(vm, index, &val))) throw TypeException("Could not get string from squirrel stack");

            if(val == nullptr)
            {
                return std::wstring(L""); 
            }

            SQInteger len = sq_getsize(vm,index);    
            return std::wstring(val,len);
        }
#else
        template<>
        inline std::string popValue(HSQUIRRELVM vm, SQInteger index){
            checkType(vm, index, OT_STRING);
            const SQChar* val;
            if (SQ_FAILED(sq_getstring(vm, index, &val))) throw TypeException("Could not get string from squirrel stack");

            if(val == nullptr)
            {
                return std::string(""); 
            }

            SQInteger len = sq_getsize(vm,index);    
            return std::string(val,len);
        }
#endif

        template <typename T> inline typename std::enable_if<!std::is_pointer<T>::value, T>::type
        pop(HSQUIRRELVM vm, SQInteger index) { 
            return popValue<typename std::remove_cv<T>::type>(vm, index); 
        }

        template <typename T> inline typename std::enable_if<std::is_pointer<T>::value, T>::type
        pop(HSQUIRRELVM vm, SQInteger index) { 
            return popPointer<T>(vm, index);
        }

        template<typename T>
        inline void pushByCopy(HSQUIRRELVM vm, const T& value) {
            static const auto hashCode = typeid(T*).hash_code();
            try {
                sq_pushobject(vm, getClassObj(vm, hashCode));
                sq_createinstance(vm, -1);
                sq_remove(vm, -2);

                sq_setinstanceup(vm, -1, reinterpret_cast<SQUserPointer>(new T(value)));
                sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(hashCode));
                sq_setreleasehook(vm, -1, classDestructor<T>);
            } catch (std::out_of_range& e) {
                (void)e;
                T** data = reinterpret_cast<T**>(sq_newuserdata(vm, sizeof(T*)));
                *data = new T(value);
                sq_setreleasehook(vm, -1, classPtrDestructor<T>);
                sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(typeid(T).hash_code()));
            }
        }

        template<typename T>
        inline void pushValue(HSQUIRRELVM vm, const T& value){
            pushByCopy<T>(vm, value);
        }

        SSQ_API void pushRaw(HSQUIRRELVM vm, const Object& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Class& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Instance& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Table& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Function& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Enum& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const Array& value);
        SSQ_API void pushRaw(HSQUIRRELVM vm, const SqWeakRef& value);

        template<>
        inline void pushValue(HSQUIRRELVM vm, const std::nullptr_t& value){
            (void)value;
            sq_pushnull(vm);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const HSQOBJECT& value){
            sq_pushobject(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Object& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Class& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Instance& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const SqWeakRef& value) {
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Table& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Function& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Enum& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const Array& value){
            pushRaw(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const bool& value) {
            sq_pushbool(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const int& value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const short& value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const long& value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const char& value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const signed char& value) {
            sq_pushinteger(vm, value);
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const unsigned int& value) {
            sq_pushinteger(vm, *reinterpret_cast<const int*>(&value));
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const unsigned short& value) {
            sq_pushinteger(vm, *reinterpret_cast<const short*>(&value));
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const unsigned long& value) {
            sq_pushinteger(vm, *reinterpret_cast<const long*>(&value));
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const unsigned char& value) {
            sq_pushinteger(vm, *reinterpret_cast<const char*>(&value));
        }

#ifdef _SQ64
        template<>
        inline void pushValue(HSQUIRRELVM vm, const unsigned long long& value) {
            sq_pushinteger(vm, *reinterpret_cast<const long long*>(&value));
        }

        template<>
        inline void pushValue(HSQUIRRELVM vm, const long long& value) {
            sq_pushinteger(vm, *reinterpret_cast<const long long*>(&value));
        }
#endif

#ifdef SQUSEDOUBLE
        template<>
        inline void pushValue(HSQUIRRELVM vm, const double& value) {
            sq_pushfloat(vm, value);
        }
#endif

        template<>
        inline void pushValue(HSQUIRRELVM vm, const float& value) {
            sq_pushfloat(vm, value);
        }

#ifdef SQUNICODE
        template<>
        inline void pushValue(HSQUIRRELVM vm, const std::wstring& value) {
            sq_pushstring(vm, value.c_str(), value.size());
        }
#else
        template<>
        inline void pushValue(HSQUIRRELVM vm, const std::string& value) {
            sq_pushstring(vm, value.c_str(), value.size());
        }
#endif

        template<typename T>
        inline void pushByPtr(HSQUIRRELVM vm, T* value) {
            static const auto hashCode = typeid(T*).hash_code();
            if (value == nullptr) {
                sq_pushnull(vm);
            }
            else {
                try {
                    sq_pushobject(vm, getClassObj(vm, hashCode));
                    sq_createinstance(vm, -1);
                    sq_remove(vm, -2);
                    sq_setinstanceup(vm, -1, (SQUserPointer)(value));
                    sq_settypetag(vm, -1, reinterpret_cast<SQUserPointer>(hashCode));
                }
                catch (std::out_of_range& e) {
                    (void)e;
                    sq_pushuserpointer(vm, (SQUserPointer)(value));
                }
            }
        }

        template <typename T, typename std::enable_if<!std::is_pointer<T>::value, T>::type* = nullptr>
        inline void push(HSQUIRRELVM vm, const T& value) { 
            pushValue<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>(vm, value); 
        }

        template <typename T, typename std::enable_if<std::is_pointer<T>::value, T>::type* = nullptr>
        inline void push(HSQUIRRELVM vm, const T& value) { 
            pushByPtr<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>(vm, value);
        }
    }
#endif
}

#endif