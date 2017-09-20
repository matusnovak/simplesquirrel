#pragma once
#ifndef SIMPLESQ_HEADER_H
#define SIMPLESQ_HEADER_H

#include <algorithm>
#include <iostream>
#include <cstdarg>
#include <type_traits>
#include <squirrel.h>
#include <sqstdstring.h>
#include <sqstdsystem.h>
#include <sqstdmath.h>
#include <sqstdblob.h>
#include <sqstdio.h>
#include <functional>
#include <unordered_map>
#include <array>
#include <vector>
#include <sstream>
#include <memory>

#if !defined(HAS_NOEXCEPT) && !defined(NOEXCEPT)
#if defined(__clang__)
#if __has_feature(cxx_noexcept)
#define HAS_NOEXCEPT
#endif
#else
#if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
    defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
#define HAS_NOEXCEPT
#endif
#endif

#ifdef HAS_NOEXCEPT
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif
#endif

#ifndef SIMPLESQ_UTF8_TO_WSTR
#define SIMPLESQ_UTF8_TO_WSTR(STR) ssq::detail::toWstring(STR)
#endif

///=============================================================================
/// 
///                                 EXCEPTIONS
///
///=============================================================================
namespace ssq {
    class Exception: public std::exception {
    public:
        Exception(const std::string& msg):message(msg){

        }
        virtual const char* what() const throw() override {
            return message.c_str();
        }
    private:
        std::string message;
    };


	class TypeException: public Exception {
    public:
        TypeException(const std::string& msg):Exception(msg){

        }
    };

    class CompileException: public Exception {
    public:
        CompileException(
            const char* err, 
            const char* source, 
            SQInteger line, SQInteger column):
            Exception(getFormated(err, source, line, column)),
            lineNum(line),columnNum(column){
        }
        SQInteger getLineNum() const {
            return lineNum;
        }
        SQInteger getColumnNum() const {
            return columnNum;
        }
    private:
        static std::string getFormated(const char* err, const char* source, SQInteger line, SQInteger column) {
            std::stringstream ss;
            ss << "Compile error at " << source << ":" << line << ":" << column << " " << err;
            return ss.str();
        }
        SQInteger lineNum;
        SQInteger columnNum;
    };

    class RuntimeException: public Exception {
    public:
        RuntimeException(const std::string& msg):Exception(msg),lineNum(0){

        }
        RuntimeException(
            const char* err,
            const char* source,
            const char* func,
            SQInteger line):
            Exception(getFormated(err, source, func, line)),
            lineNum(line){
        }

        SQInteger getLineNum() const {
            return lineNum;
        }
    private:
        static std::string getFormated(const char* err, const char* source, const char* func, SQInteger line) {
            std::stringstream ss;
            ss << "Runtime error at (" << func << ") " << source << ":" << line << ":" << " " << err;
            return ss.str();
        }
        SQInteger lineNum;
    };

    class NotFoundException: public Exception {
    public:
        NotFoundException(const char* msg):Exception(msg){
        }
    };

///=============================================================================
/// 
///                                 OBJECT
///
///=============================================================================
	/**
	* @brief Basic Squirrel object class
	*/
	class Object {
	public:
		/**
		* @brief Constructs an empty squirrel class from VM
		*/
        Object(HSQUIRRELVM v){
			vm = v;
			sq_resetobject(&obj);
		}
		/**
		* @brief Copies the reference of an other object
		*/
        Object(const Object& other) {
			vm = other.vm;
			obj = other.obj;
			if(vm != nullptr && !other.isEmpty()) {
				sq_addref(vm, &obj);
			}
		}
		/**
		* @brief Move constructor
		*/
        Object(Object&& other) NOEXCEPT {
			vm = nullptr;
			sq_resetobject(&obj);
            swap(other);
        }
        virtual ~Object(){
            destroy();
		}
		/**
		* @brief Swapts the contents of this squirrel object with another one
		*/
        void swap(Object& other) NOEXCEPT {
        	using std::swap;
            swap(obj, other.obj);
            swap(vm, other.vm);
		}
		/**
		* @brief Destroys the object, otherwise does nothing
		*/
        void destroy(){
            if(vm != nullptr && !sq_isnull(obj)) {
				sq_release(vm, &obj);
                sq_resetobject(&obj);
            } 
		}
		/**
		* @brief Returns a reference to squirrel object handle
		*/
        HSQOBJECT& getObj() {
            return obj;
		}
		/**
		* @brief Returns a const reference to squirrel object handle
		*/
        const HSQOBJECT& getObj() const {
            return obj;
		}
		/**
		* @brief Returns a VM that was assigned to this object in the constructor
		*/
		HSQUIRRELVM getVM() const {
			return vm;
		}
		/**
		* @brief Returns true if this object is empty
		*/
        bool isEmpty() const {
            return sq_isnull(obj);
        }
        Object& operator = (const Object& other) {
			if(this != &other) {
				Object o(other);
				swap(o);
			}
			return *this;
        }
        Object& operator = (Object&& other) NOEXCEPT {
            if(this != &other){
                swap(other);
            }
            return *this;
        }
    protected:
        HSQUIRRELVM vm;
        HSQOBJECT obj;
    };

	template<class Signature>
	struct Constructor;

	template<class T, class... Args>
	struct Constructor<T(Args...)> {
		static T* allocate(Args&&... args) {
			return new T(std::forward<Args>(args)...);
		}
	};

	/**
	* @brief Iterator wrapper to be used to transfer arrays into squirrel
	*/
	template <class Iter>
	class Iterator {
	public:
		Iterator(
			Iter a, 
			Iter b)
			:begin(a),end(b) {
		}
		Iter begin;
		Iter end;
	};

	/**
	* @brief Raw C-array wrapper to be used to transfer arrays into squirrel
	*/
	template <class T>
	class RawArray {
	public:
		RawArray(const T* ptr, size_t len):ptr(ptr),len(len) {
			
		}

		const T* ptr;
		const size_t len;
	};

///=============================================================================
/// 
///                                 INTERNAL
///
///=============================================================================
	class detail {
	public:
		typedef std::unordered_map<size_t, HSQOBJECT> ClassMap;

		template<class T>
		static const HSQOBJECT& getClassObject(HSQUIRRELVM vm);

		template<class T>
		static HSQOBJECT& getClassObjectOrCreate(HSQUIRRELVM vm);

		template<class T> struct ParamType {
			static const char value = '.';
		};

		template<class T> struct ParamType<T*> {
			static const char value = 'x';
		};

		template<class T> struct ParamType<std::vector<T>> {
			static const char value = 'a';
		};

		template<class T> struct ParamType<const std::vector<T>&> {
			static const char value = 'a';
		};

		template <class A>
		static void paramPackerType(char* ptr) {
			*ptr = ParamType<A>::value;
		}

		template <class ...B>
		static void paramPacker(char* ptr) {
			int _[] = { 0, (paramPackerType<B>(ptr++), 0)... };
			(void)_;
			*ptr = '\0';
		}

		//template<class Signature>
        //struct FuncPtr;

        template<class Ret>
		struct FuncPtr {
			const std::function<Ret()>* ptr;
		};

		template<class Ret, typename... Args>
		struct FuncPtr<Ret(Args...)> {
			const std::function<Ret(Args...)>* ptr;
		};

		template<class T>
		static SQInteger classRelease(SQUserPointer ptr, SQInteger size) {
			T *p = static_cast<T*>(ptr);
			delete p;
			return 0;
		}

		template<class T>
		struct push {
			// Push class by copy
			static void f(HSQUIRRELVM vm, T val) {
				sq_pushobject(vm, getClassObject<T>(vm));
				sq_createinstance(vm, -1);
				sq_remove(vm, -2);

				sq_setinstanceup(vm, -1, (SQUserPointer)(new T(val)));
				sq_setreleasehook(vm, -1, classRelease<T>);
			}
		};

		template<class T>
		struct push<const T&> {
			static void f(HSQUIRRELVM vm, const T& val) {
				sq_pushobject(vm, getClassObject<T>(vm));
				sq_createinstance(vm, -1);
				sq_remove(vm, -2);

				sq_setinstanceup(vm, -1, (SQUserPointer)(&val));
			}
		};

		template<class T>
		struct push<T*> {
			static void f(HSQUIRRELVM vm, T* val) {
				if(val == nullptr) {
					sq_pushnull(vm);
				} else {
					sq_pushobject(vm, getClassObject<T>(vm));
					sq_createinstance(vm, -1);
					sq_remove(vm, -2);

					sq_setinstanceup(vm, -1, (SQUserPointer)(val));
				}
			}
		};

		template<class T>
		struct push<T* const&> {
			static void f(HSQUIRRELVM vm, T* const& val) {
				if(val == nullptr) {
					sq_pushnull(vm);
				} else {
					sq_pushobject(vm, getClassObject<T>(vm));
					sq_createinstance(vm, -1);
					sq_remove(vm, -2);

					sq_setinstanceup(vm, -1, (SQUserPointer)(val));
				}
			}
		};

		template<class Iter>
		struct push<Iterator<Iter>> {
			static void f(HSQUIRRELVM vm, Iterator<Iter> val) {
				sq_newarray(vm, 0);

				for (auto it = val.begin; it != val.end; ++it) {
					push<typename std::iterator_traits<Iter>::value_type>::f(vm, *it);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T>
		struct push<RawArray<T>> {
			static void f(HSQUIRRELVM vm, RawArray<T> val) {
				sq_newarray(vm, 0);

				for (size_t i = 0; i < val.len; i++) {
					push<T>::f(vm, val.ptr[i]);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T>
		struct push<std::vector<T>> {
			static void f(HSQUIRRELVM vm, std::vector<T> val) {
				sq_newarray(vm, 0);

				for (auto it = val.begin(); it != val.end(); ++it) {
					push<T>::f(vm, *it);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T>
		struct push<const std::vector<T>&> {
			static void f(HSQUIRRELVM vm, const std::vector<T>& val) {
				sq_newarray(vm, 0);

				for (auto it = val.begin(); it != val.end(); ++it) {
					push<T>::f(vm, *it);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T, size_t N>
		struct push<std::array<T, N>> {
			static void f(HSQUIRRELVM vm, std::array<T, N> val) {
				sq_newarray(vm, 0);

				for (auto it = val.begin(); it != val.end(); ++it) {
					push<T>::f(vm, *it);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T, size_t N>
		struct push<const std::array<T, N>&> {
			static void f(HSQUIRRELVM vm, const std::array<T, N>& val) {
				sq_newarray(vm, 0);

				for (auto it = val.begin(); it != val.end(); ++it) {
					push<T>::f(vm, *it);
					sq_arrayappend(vm, -2);
				}
			}
		};

		template<class T>
		struct pop {
			// Pop class by copy
			static T f(HSQUIRRELVM vm, int index) {
				auto type = sq_gettype(vm, index);
				if (type == OT_INSTANCE) {
					size_t tag;
					sq_gettypetag(vm, index, (SQUserPointer*)&tag);
					if(tag != typeid(T).hash_code()) {
						throw TypeException("Invalid class type passed from squirrel 0");
					}

					SQUserPointer val;
					if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
						throw TypeException("Could not get instance from squirrel stack");
					}

					// Create copy of instance
					return T(*reinterpret_cast<T*>(val));
				} else {
					throw TypeException("Expected instance of class");
				}
			}
		};

		template<class T>
		struct pop<T*> {
			static T* f(HSQUIRRELVM vm, int index) {
				SQUserPointer val;
				auto type = sq_gettype(vm, index);
				if(type == OT_USERPOINTER) {
					if (SQ_FAILED(sq_getuserpointer(vm, index, &val)))
						throw TypeException("Could not get user pointer from squirrel stack");
					return reinterpret_cast<T*>(val);
				} else {
					size_t tag;
					sq_gettypetag(vm, index, (SQUserPointer*)&tag);
					if(tag != typeid(T).hash_code()) {
						throw TypeException("Invalid class type passed from squirrel");
					}
					if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
						throw TypeException("Could not get instance from squirrel stack");
					}
					return reinterpret_cast<T*>(val);
				}
			}
		};

		template<class T>
		struct pop<T* const&> {
			static T* f(HSQUIRRELVM vm, int index) {
				SQUserPointer val;
				auto type = sq_gettype(vm, index);
				if(type == OT_USERPOINTER) {
					if (SQ_FAILED(sq_getuserpointer(vm, index, &val)))
						throw TypeException("Could not get user pointer from squirrel stack");
					return reinterpret_cast<T*>(val);
				} else {
					size_t tag;
					sq_gettypetag(vm, index, (SQUserPointer*)&tag);
					if(tag != typeid(T).hash_code()) {
						throw TypeException("Invalid class type passed from squirrel");
					}
					if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
						throw TypeException("Could not get instance from squirrel stack");
					}
					return reinterpret_cast<T*>(val);
				}
			}
		};

		template<class T>
		struct pop<const T&> {
			// Returning const ref from C++ function into squirrel and back is
			// not supported, copy is only solution
			static T f(HSQUIRRELVM vm, int index) {
				SQUserPointer val;
				auto type = sq_gettype(vm, index);
				if(type == OT_USERPOINTER) {
					if (SQ_FAILED(sq_getuserpointer(vm, index, &val)))
						throw TypeException("Could not get user pointer from squirrel stack");
					return *reinterpret_cast<T*>(val);
				} else {
					size_t tag;
					sq_gettypetag(vm, index, (SQUserPointer*)&tag);
					if(tag != typeid(T).hash_code()) {
						throw TypeException("Invalid class type passed from squirrel");
					}
					if (SQ_FAILED(sq_getinstanceup(vm, index, &val, nullptr))) {
						throw TypeException("Could not get instance from squirrel stack");
					}
					return *reinterpret_cast<T*>(val);
				}
				//throw TypeException("Can't get const reference of squirrel stack! Use pointers instead. This happens when you try to call C++ function that accepts const reference.");
			}
		};

		template<class T>
		struct pop<std::vector<T>> {
			static std::vector<T> f(HSQUIRRELVM vm, int index) {
				auto type = sq_gettype(vm, index);
				if (type != OT_ARRAY)
					throw TypeException("Could not get array from squirrel stack");

				size_t len = sq_getsize(vm, index);
				std::vector<T> vec;
				vec.reserve(len);

				for(size_t i = 0; i < len; i++) {
					sq_arraypop(vm, index, true);
					vec.insert(vec.begin(), std::forward<T>(pop<T>::f(vm, -1)));
					sq_pop(vm, 1);
				}

				return vec;
			}
		};

		template<class T>
		struct pop<const std::vector<T>&> {
			static std::vector<T> f(HSQUIRRELVM vm, int index) {
				auto type = sq_gettype(vm, index);
				if (type != OT_ARRAY)
					throw TypeException("Could not get array from squirrel stack");

				size_t len = sq_getsize(vm, index);
				std::vector<T> vec;
				vec.reserve(len);

				for(size_t i = 0; i < len; i++) {
					sq_arraypop(vm, index, true);
					vec.insert(vec.begin(), std::forward<T>(pop<T>::f(vm, -1)));
					sq_pop(vm, 1);
				}

				return vec;
			}
		};

		template<class Signature>
		struct pushReturnVal;

		template<class... Args>
		struct pushReturnVal<void(Args...)> {
			static int f(HSQUIRRELVM vm, const std::function<void(Args...)>* func, Args&&... args) {
				func->operator()(std::forward<Args>(args)...);
				(void)vm;
				return 0;
			}
		};

		template<class Ret, class... Args>
		struct pushReturnVal<Ret(Args...)> {
			static int f(HSQUIRRELVM vm, const std::function<Ret(Args...)>* func, Args&&... args) {
				Ret ret = func->operator()(std::forward<Args>(args)...);
				push<Ret>::f(vm, ret);
				return 1;
			}
		};

		template<int offset, class Ret, class... Args>
		struct globalFunc {
			static SQInteger f(HSQUIRRELVM vm) {
				try {
					static const std::size_t nparams = sizeof...(Args);

					FuncPtr<Ret(Args...)>* funcPtr;
					sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);

					int index = nparams + offset;
					push<Ret>::f(vm, std::forward<Ret>(funcPtr->ptr->operator()(pop<Args>::f(vm, index--)...)));
					return 1;
				} catch (std::exception& e) {
					return sq_throwerror(vm, e.what());
				}
			}
		};

		template<int offset, class... Args>
		struct globalFunc<offset, void, Args...> {
			static SQInteger f(HSQUIRRELVM vm) {
				try {
					static const std::size_t nparams = sizeof...(Args);

					FuncPtr<void(Args...)>* funcPtr;
					sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);

					int index = nparams + offset;
					funcPtr->ptr->operator()(pop<Args>::f(vm, index--)...);
					return 0;
				} catch (std::exception& e) {
					return sq_throwerror(vm, e.what());
				}
			}
		};

		template<class T, class... Args>
		static SQInteger classAllocator(HSQUIRRELVM vm) {
			static const std::size_t nparams = sizeof...(Args);
			int off = nparams;

			FuncPtr<T*(Args...)>* funcPtr;
			sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);
			int index = nparams + 1;

			T* p = funcPtr->ptr->operator()(pop<Args>::f(vm, index--)...);
			sq_setinstanceup(vm, -2 -off, p);
			sq_setreleasehook(vm, -2 -off, &detail::classRelease<T>);

			sq_getclass(vm, -2 -off);
			sq_settypetag(vm, -1, (SQUserPointer)typeid(T).hash_code());
			sq_pop(vm, 1); // Pop class
			return nparams;
		}

		template<class T, class... Args>
		static SQInteger classAllocatorNoRelease(HSQUIRRELVM vm) {
			static const std::size_t nparams = sizeof...(Args);
			int off = nparams;

			FuncPtr<T*(Args...)>* funcPtr;
			sq_getuserdata(vm, -1, reinterpret_cast<void**>(&funcPtr), nullptr);
			int index = nparams + 1;

			T* p = funcPtr->ptr->operator()(pop<Args>::f(vm, index--)...);
			sq_setinstanceup(vm, -2 -off, p);

			sq_getclass(vm, -2 -off);
			sq_settypetag(vm, -1, (SQUserPointer)typeid(T).hash_code());
			sq_pop(vm, 1); // Pop class
			return nparams;
		}

		template<class Ret, class... Args>
		static SQInteger funcReleaseHook(SQUserPointer p, SQInteger size) {
			auto funcPtr = reinterpret_cast<FuncPtr<Ret(Args...)>*>(p);
			delete funcPtr->ptr;
			return 0;
		}

		template<class Ret, class... Args>
		static void bindUserData(HSQUIRRELVM vm, const std::function<Ret(Args...)>& func) {
			auto funcStruct = reinterpret_cast<detail::FuncPtr<Ret(Args...)>*>(sq_newuserdata(vm, sizeof(detail::FuncPtr<Ret(Args...)>)));
			funcStruct->ptr = new std::function<Ret(Args...)>(func);
			sq_setreleasehook(vm, -1, &detail::funcReleaseHook<Ret, Args...>);
		}

		template<class Ret, class... Args>
		static void bindFunc(HSQUIRRELVM vm, const std::string& name, const std::function<Ret(Args...)>& func) {
			static const std::size_t nparams = sizeof...(Args);

			sq_pushstring(vm, name.c_str(), name.size());

			bindUserData(vm, func);
			static char params[33];
			paramPacker<void, Args...>(params);

			sq_newclosure(vm, &detail::globalFunc<1, Ret, Args...>::f, 1);
			sq_setparamscheck(vm, nparams + 1, params);
			sq_newslot(vm, -3, SQFalse);
		}

		template<class Ret, class T, class... Args>
		static void bindMemFunc(HSQUIRRELVM vm, const std::string& name, const std::function<Ret(T*, Args...)>& func, bool isStatic = false) {
			static const std::size_t nparams = sizeof...(Args);

			sq_pushstring(vm, name.c_str(), name.size());

			bindUserData(vm, func);
			static char params[33];
			paramPacker<T*, Args...>(params);

			sq_newclosure(vm, &detail::globalFunc<0, Ret, T*, Args...>::f, 1);
			sq_setparamscheck(vm, nparams + 1, params);
			sq_newslot(vm, -3, isStatic);
		}

		template<class T, class... Args>
		static void bindClass(HSQUIRRELVM vm, const std::string& name, const std::function<T*(Args...)>& allocator, HSQOBJECT& clsObj, bool release = true) {
			static const std::size_t nparams = sizeof...(Args);
			
			sq_pushstring(vm, name.c_str(), name.size());
			sq_newclass(vm, false);

			HSQOBJECT& obj = getClassObjectOrCreate<T>(vm);
			sq_getstackobj(vm, -1, &obj);
			sq_addref(vm, &obj);

			sq_getstackobj(vm, -1, &clsObj);
			sq_addref(vm, &clsObj);

			sq_settypetag(vm, -1, (SQUserPointer)typeid(T).hash_code());

			sq_pushstring(vm, "constructor", -1);
			bindUserData<T*>(vm, allocator);
			static char params[33];
			paramPacker<T*, Args...>(params);

			if(release) {
				sq_newclosure(vm, &detail::classAllocator<T, Args...>, 1);
			} else {
				sq_newclosure(vm, &detail::classAllocatorNoRelease<T, Args...>, 1);
			}

			sq_setparamscheck(vm, nparams + 1, params);
			sq_newslot(vm, -3, false); // Add the constructor method

			sq_newslot(vm, -3, SQFalse); // Add the class
		}

		template<class T, class... Args>
		static void bindAbstractClass(HSQUIRRELVM vm, const std::string& name, HSQOBJECT& clsObj) {
			static const std::size_t nparams = sizeof...(Args);
			
			sq_pushstring(vm, name.c_str(), name.size());
			sq_newclass(vm, false);

			HSQOBJECT& obj = getClassObjectOrCreate<T>(vm);
			sq_getstackobj(vm, -1, &obj);
			sq_addref(vm, &obj);

			sq_getstackobj(vm, -1, &clsObj);
			sq_addref(vm, &clsObj);

			sq_settypetag(vm, -1, (SQUserPointer)typeid(T).hash_code());

			sq_newslot(vm, -3, SQFalse); // Add the class
		}

		template<class T>
		static T* defaultClassAllocator() {
			return new T();
        }

		static std::wstring toWstring(const SQChar* str) {
			size_t len = strlen(str);
			std::wstring wstr;
			wstr.resize(len);
			for(size_t i = 0; i < len; i++) {
				wstr[i] = str[i];
			}
			return wstr;
		}
	private:

	};

///=============================================================================
/// 
///                      INTERNAL PUSH FUNCTIONS
///
///=============================================================================
	template<>
    inline void detail::push<int8_t>::f(HSQUIRRELVM vm, int8_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<int16_t>::f(HSQUIRRELVM vm, int16_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<int32_t>::f(HSQUIRRELVM vm, int32_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<int64_t>::f(HSQUIRRELVM vm, int64_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<uint8_t>::f(HSQUIRRELVM vm, uint8_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<uint16_t>::f(HSQUIRRELVM vm, uint16_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<uint32_t>::f(HSQUIRRELVM vm, uint32_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<uint64_t>::f(HSQUIRRELVM vm, uint64_t val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

    template<>
    inline void detail::push<float>::f(HSQUIRRELVM vm, float val){
        sq_pushfloat(vm, static_cast<SQFloat>(val));
    }

	template<>
    inline void detail::push<double>::f(HSQUIRRELVM vm, double val){
        sq_pushfloat(vm, static_cast<SQFloat>(val));
    }

    template<>
    inline void detail::push<std::string>::f(HSQUIRRELVM vm, std::string val){
        sq_pushstring(vm, val.c_str(), val.size());
    }

    template<>
    inline void detail::push<bool>::f(HSQUIRRELVM vm, bool val){
        sq_pushbool(vm, val);
    }

	template<>
    inline void detail::push<void*>::f(HSQUIRRELVM vm, void* val){
        sq_pushuserpointer(vm, (SQUserPointer)val);
    }

	template<>
    inline void detail::push<const int8_t&>::f(HSQUIRRELVM vm, const int8_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const int16_t&>::f(HSQUIRRELVM vm, const int16_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const int32_t&>::f(HSQUIRRELVM vm, const int32_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const int64_t&>::f(HSQUIRRELVM vm, const int64_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const uint8_t&>::f(HSQUIRRELVM vm, const uint8_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const uint16_t&>::f(HSQUIRRELVM vm, const uint16_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const uint32_t&>::f(HSQUIRRELVM vm, const uint32_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

	template<>
    inline void detail::push<const uint64_t&>::f(HSQUIRRELVM vm, const uint64_t& val){
        sq_pushinteger(vm, static_cast<SQInteger>(val));
    }

    template<>
    inline void detail::push<const float&>::f(HSQUIRRELVM vm, const float& val){
        sq_pushfloat(vm, static_cast<SQFloat>(val));
    }

	template<>
    inline void detail::push<const double&>::f(HSQUIRRELVM vm, const double& val){
        sq_pushfloat(vm, static_cast<SQFloat>(val));
    }

    template<>
    inline void detail::push<const std::string&>::f(HSQUIRRELVM vm, const std::string& val){
        sq_pushstring(vm, val.c_str(), val.size());
    }

    template<>
    inline void detail::push<const bool&>::f(HSQUIRRELVM vm, const bool& val){
        sq_pushbool(vm, val);
    }

	template<>
    inline void detail::push<void* const&>::f(HSQUIRRELVM vm, void* const& val){
        sq_pushuserpointer(vm, (SQUserPointer)val);
    }

	template<>
    inline void detail::push<void const*>::f(HSQUIRRELVM vm, void const* val){
        sq_pushuserpointer(vm, (SQUserPointer)val);
    }

///=============================================================================
/// 
///                      CALL RETURN VALUE
///
///=============================================================================
	template<class T>
    class RetVal {
    public:
        RetVal(HSQUIRRELVM vm, int index){
            value = detail::pop<T>::f(vm, index);
        }
        operator T() const {
			return value;
		}
        T value;
    };

    template<>
    class RetVal<void> {
    public:
        RetVal(HSQUIRRELVM vm, int index){
            (void)vm;
            (void)index;
        }
        operator void() const {
            // Do nothing
		}
    };

///=============================================================================
/// 
///                                 SCRIPT
///
///=============================================================================
	/**
	* @brief Class that holds squirrel script 
	*/
	class Script : public Object {
	public:
		Script(HSQUIRRELVM vm) :Object(vm) {

		}
		Script(const Script& other) = delete;
		Script(Script&& other) NOEXCEPT :Object(std::forward<Script>(other)) {
		}
		virtual ~Script() {

		}
		Script& operator = (const Script& other) = delete;
		Script& operator = (Script&& other) NOEXCEPT {
			if (this != &other) {
				Object::swap(other);
			}
			return *this;
		}
	};

///=============================================================================
/// 
///                                 ENUM
///
///=============================================================================
	/**
	* @brief Enum squirrel type
	*/
	class Enum: public Object {
    public:
        Enum(HSQUIRRELVM vm):Object(vm){
            sq_newtable(vm);
            sq_getstackobj(vm, -1, &obj);
            sq_addref(vm, &obj);
            sq_pop(vm,1); // Pop enum table
        }
        Enum(const Enum& other):Object(other) {
	        
        }
        Enum(Enum&& other) NOEXCEPT :Object(std::forward<Enum>(other)) {
        }
        virtual ~Enum(){

		}
		/**
		* @brief Adds a constant integer named value as an enum slot
		*/
        void addSlot(const std::string& name, int val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<int>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
		}
		/**
		* @brief Adds a constant float named value as an enum slot
		*/
        void addSlot(const std::string& name, float val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<float>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
		}
		/**
		* @brief Adds a constant string named value as an enum slot
		*/
        void addSlot(const std::string& name, const std::string& val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<std::string>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
        }
        Enum& operator = (const Enum& other) {
			Object::operator=(other);
			return *this;
        }
        Enum& operator = (Enum&& other) NOEXCEPT {
            if(this != &other){
                Object::swap(other);
            }
            return *this;
        }
    };

	template<>
    inline void detail::push<const Enum&>::f(HSQUIRRELVM vm, const Enum& val){
        sq_pushobject(vm, val.getObj());
    }

///=============================================================================
/// 
///                                 TABLE
///
///=============================================================================
	/**
	* @brief Squirrel table data type
	*/
	class Table: public Object {
    public:
        Table(HSQUIRRELVM vm, bool create = true):Object(vm){
			if (vm != nullptr && create) {
				sq_newtable(vm);
				sq_getstackobj(vm, -1, &obj);
				sq_addref(vm, &obj);
				sq_pop(vm, 1); // Pop table
			}
        }
        Table(const Table& other):Object(other) {
	        
        }
        Table(Table&& other) NOEXCEPT :Object(std::forward<Table>(other)) {
        }
        virtual ~Table(){

		}
		/**
		* @brief Adds a constant integer named value as an enum slot
		*/
        void addSlot(const std::string& name, int val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<int>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
		}
		/**
		* @brief Adds a constant float named value as an enum slot
		*/
        void addSlot(const std::string& name, float val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<float>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
		}
		/**
		* @brief Adds a constant string named value as an enum slot
		*/
        void addSlot(const std::string& name, const std::string& val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<std::string>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
        }
        Table& operator = (const Table& other) {
			Object::operator=(other);
			return *this;
        }
        Table& operator = (Table&& other) NOEXCEPT {
            if(this != &other){
                Object::swap(other);
            }
            return *this;
        }
    };

	template<>
    inline void detail::push<const Table&>::f(HSQUIRRELVM vm, const Table& val){
        sq_pushobject(vm, val.getObj());
    }

///=============================================================================
/// 
///                                 FUNCTION
///
///=============================================================================
	/**
	* @brief Squirrel function data type
	*/
	class Function: public Object {
    public:
		Function():Object(nullptr) {
			nparams = 0;
		}
        Function(HSQUIRRELVM vm, SQUnsignedInteger params):Object(vm){
			nparams = params;
        }
        Function(const Function& other):Object(other) {
	        nparams = other.nparams;
        }
        Function(Function&& other) NOEXCEPT :Object(std::forward<Function>(other)) {
        	std::swap(nparams, other.nparams);
        }
        virtual ~Function(){
		}
		/**
		* @brief Returns the number of arguments used by the function excluding this pointer
		*/
		inline SQUnsignedInteger getNumOfParams() const {
            return nparams;
		}
		/**
		* @brief Returns the name of the function
		*/
		std::string getName() const {
			const SQChar* str;
			sq_pushobject(vm, obj);
			sq_getclosurename(vm, -1);
			sq_getstring(vm, -1, &str);
			sq_pop(vm, 2);
			return std::string(str);
        }
        Function& operator = (const Function& other){
			Object::operator=(other);
			nparams = other.nparams;
			return *this;
        }
        Function& operator = (Function&& other) NOEXCEPT {
            if(this != &other){
                Object::swap(other);
				std::swap(nparams, other.nparams);
            }
            return *this;
        }
	protected:
        SQUnsignedInteger nparams;
    };

///=============================================================================
/// 
///                                 RAW CLASS
///
///=============================================================================
	/**
	* @brief Squirrel class data type
	*/
	class RawClass: public Object {
    public:
        RawClass(HSQUIRRELVM vm):Object(vm){

        }
        RawClass(const RawClass& other):Object(other) {
	        
        }
        RawClass(RawClass&& other) NOEXCEPT :Object(std::forward<RawClass>(other)) {
        }
        virtual ~RawClass(){

        }
        RawClass& operator = (const RawClass& other){
			Object::operator=(other);
			return *this;
        }
        RawClass& operator = (RawClass&& other) NOEXCEPT {
            if(this != &other){
                Object::swap(other);
            }
            return *this;
        }
		/**
		* @brief Adds enum to this class as a static variable
		*/
		Enum addEnum(const std::string& name){
            Enum enm(vm);

            // Register enum to class table
            addConst(name, enm);
            return enm;
        }
		/**
		* @brief Adds a constant value to this class as a static variable
		*/
        template<class V>
        void addConst(const std::string& name, const V& val){
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<const V&>::f(vm, val);
            sq_newslot(vm, -3, true);
            sq_pop(vm,1); // Pop class table
        }
		/**
		* @brief Returns type tag used by the class which is equal to hash value of typeid(T)
		*/
		size_t getTypeTag() const {
			SQUserPointer tag;
	        sq_pushobject(vm, obj);
			sq_gettypetag(vm, -1, &tag);
			sq_pop(vm, 1);
			return reinterpret_cast<size_t>(tag);
        }
		/**
		* @brief Finds a function that exists in this class
		* @details Throws NotFoundException if not found or TypeException if the found
		* object is not a function
		*/
		Function findFunc(const std::string& name) const {
            sq_pushobject(vm, obj);
            sq_pushstring(vm, name.c_str(), name.size());

            if(SQ_FAILED(sq_get(vm, -2))) {
                sq_pop(vm, 1);
                throw NotFoundException("Function not found");
            }

            SQObjectType value_type = sq_gettype(vm, -1);
            if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
                sq_pop(vm, 2);
                throw TypeException("Not a function");
            }

            SQUnsignedInteger nparams;
            SQUnsignedInteger nfreevars;
            if (SQ_FAILED(sq_getclosureinfo(vm, -1, &nparams, &nfreevars))) {
                sq_pop(vm, 2);
                throw TypeException("Get function info failed");
            }

            Function func(vm, nparams -1);
            sq_getstackobj(vm, -1, &func.getObj());
            sq_addref(vm, &func.getObj());
            sq_pop(vm, 2);

            return func;
        }
    };

	template<>
    inline void detail::push<const RawClass&>::f(HSQUIRRELVM vm, const RawClass& val){
        sq_pushobject(vm, val.getObj());
    }

///=============================================================================
/// 
///                                 CLASS
///
///=============================================================================
	/**
	* @brief Squirrel class data type from already existing C++ class
	* @details You can create this class by calling addClass() function
	* from ssq::Machine instance.
	*/
	template<class T>
    class Class: public RawClass {
    public:
        Class(HSQUIRRELVM vm):RawClass(vm){
			sq_resetobject(&tables.get);
			sq_resetobject(&tables.set);
        }
        Class(const Class& other):RawClass(other) {
	        sq_resetobject(&tables.get);
			sq_resetobject(&tables.set);
        }
        Class(Class&& other) NOEXCEPT :RawClass(std::forward<Class>(other)) {
			using std::swap;
			swap(tables.get, other.tables.get);
			swap(tables.set, other.tables.set);
        }
        virtual ~Class(){

        }
		void swap(Class& other) {
			RawClass::swap(other);
			using std::swap;
			swap(tables.get, other.tables.get);
			swap(tables.set, other.tables.set);
        }
        Class& operator = (const Class& other) {
			Object::operator=(other);
			if(this != &other) {
				sq_resetobject(&tables.get);
				sq_resetobject(&tables.set);
			}
			return *this;
        }
        Class& operator = (Class&& other) NOEXCEPT {
            if(this != &other){
				this->swap(other);
            }
            return *this;
        }
		/**
		* @brief Adds a member function to this class
		* @details The member function has to be a valid C++ member function.
		* Calling this function within squirrel will also pass a valid "this"
		* pointer to the function.
		* class MyClass {
		* public:
		*     MyClass(float initial){
		*	      myMemVar = initial;
		*     }
		*     float myMemFunc(float f){
		* 	      return myMemVar * f;
		*     }
		* private:
		*     float myMemVar;
		* };
		* auto myClass = vm.addClass<MyClass>("MyClass", ssq::Constructor<MyClass(float)>());
		* auto myClassFunc = myClass.addFunc("myMemFunc",
		*	std::function<float(MyClass*, float)>([](MyClass* ptr, float f){
		*     return ptr->myMemFunc(f);
		* }));
		*/
		template<class Ret, class... Args>
		void addFunc(const std::string& name, const std::function<Ret(T*, Args...)>& func){
            sq_pushobject(vm, obj);
            detail::bindMemFunc(vm, name, func, false);
            sq_pop(vm, 1);
        }
		/**
		* @brief Adds a member function to this class
		* @details The member function has to be a valid C++ member function.
		* Calling this function within squirrel will also pass a valid "this"
		* pointer to the function.
		* class MyClass {
		* public:
		*     MyClass(float initial){
		*	      myMemVar = initial;
		*     }
		*     float myMemFunc(float f){
		* 	      return myMemVar * f;
		*     }
		* private:
		*     float myMemVar;
		* };
		* auto myClass = vm.addClass<MyClass>("MyClass", ssq::Constructor<MyClass(float)>());
		* auto myClassFunc = myClass.addFunc<float>("myMemFunc",
		*	[](MyClass* ptr, float f){
		*     return ptr->myMemFunc(f);
		* });
		*/
		template<class Ret, class... Args, typename F>
		void addFunc(const std::string& name, F&& lambda){
            sq_pushobject(vm, obj);
            detail::bindMemFunc(vm, name, std::function<Ret(T*, Args...)>(lambda), false);
            sq_pop(vm, 1);
        }
		/**
		* @brief Adds a member function to this class
		* @details The member function has to be a valid C++ member function.
		* Calling this function within squirrel will also pass a valid "this"
		* pointer to the function.
		* class MyClass {
		* public:
		*     MyClass(float initial){
		*	      myMemVar = initial;
		*     }
		*     float myMemFunc(float f){
		* 	      return myMemVar * f;
		*     }
		* private:
		*     float myMemVar;
		* };
		* auto myClass = vm.addClass<MyClass>("MyClass", ssq::Constructor<MyClass(float)>());
		* auto myClassFunc = myClass.addFunc("myMemFunc", &MyClass:myMemFunc);
		*/
		template<class Ret, class... Args>
		void addFunc(const std::string& name, Ret (T::*memfunc)(Args...)) {
			sq_pushobject(vm, obj);
            detail::bindMemFunc(vm, name, std::function<Ret(T*, Args...)>( std::mem_fn(memfunc) ), false);
            sq_pop(vm, 1);
		}
		/**
		* @brief Adds a member variable to this class
		* @details The variable will be assigned via a pointer and modifying this
		* variable inside of squirrel script will also modify the real variable
		*/
		template<class V>
		void addVar(const std::string& name, V T::* ptr, bool isStatic = false) {
			auto getter = findTable("_get", tables.get, dlgGetStub);
			auto setter = findTable("_set", tables.set, dlgSetStub);

			bindVar(name, ptr, getter, varGetStub<V>, isStatic);
			bindVar(name, ptr, setter, varSetStub<V>, isStatic);
        }
		/**
		* @brief Adds a constant member variable to this class
		* @details As with addVar() this method has sme behavior. However,
		* the variable cannot be modified within squirrel script.
		*/
		template<class V>
		void addConstVar(const std::string& name, V T::* ptr, bool isStatic = false) {
			auto getter = findTable("_get", tables.get, dlgGetStub);

			bindVar(name, ptr, getter, varGetStub<V>, isStatic);
        }
	private:
		template<class V>
		void bindVar(const std::string& name, V T::* ptr, HSQOBJECT& table, SQFUNCTION stub, bool isStatic) {			
			auto rst = sq_gettop(vm);
			
			sq_pushobject(vm, table);

			sq_pushstring(vm, name.c_str(), name.size());

			SQUserPointer vp = sq_newuserdata(vm, sizeof(ptr));
			std::memcpy(vp, &ptr, sizeof(ptr));

			sq_newclosure(vm, stub, 1);

			if(SQ_FAILED(sq_newslot(vm, -3, isStatic))) {
				throw TypeException("Failed to bind var \"" + name + "\" to class");
			}
			sq_pop(vm, 1);

			sq_settop(vm, rst);
		}

		template<class V>
		static SQInteger varGetStub(HSQUIRRELVM vm) {
			T* ptr;
			sq_getinstanceup(vm, 1, (SQUserPointer*)&ptr, nullptr);

			typedef V T::*M;
			M* memberPtr = nullptr;
			sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr);
			M member = *memberPtr;

			detail::push<V>::f(vm, ptr->*member);
			return 1;
		}

		template<class V>
		static SQInteger varSetStub(HSQUIRRELVM vm) {
			T* ptr;
			sq_getinstanceup(vm, 1, (SQUserPointer*)&ptr, nullptr);

			typedef V T::*M;
			M* memberPtr = nullptr;
			sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, nullptr);
			M member = *memberPtr;

			ptr->*member = detail::pop<V>::f(vm, 2);
			return 0;
		}

		static SQInteger dlgGetStub(HSQUIRRELVM vm) {
			// Find the set method in the set table
			sq_push(vm, 2);
			if (!SQ_SUCCEEDED(sq_get(vm, -2))) {
				const SQChar* s;
				sq_getstring(vm, 2, &s);
				return sq_throwerror(vm, "Variable not found");
			}

			// push 'this'
			sq_push(vm, 1);

			// Call the setter
			sq_call(vm, 1, SQTrue, SQTrue);
			return 1;
		}

		static SQInteger dlgSetStub(HSQUIRRELVM vm) {
			sq_push(vm, 2);
			if (!SQ_SUCCEEDED(sq_get(vm, -2))) {
				const SQChar* s;
				sq_getstring(vm, 2, &s);
				return sq_throwerror(vm, "Variable  not found");
			}

			sq_push(vm, 1);

			// Call the getter
			sq_push(vm, 3);
			sq_call(vm, 2, SQTrue, SQTrue);
			return 1;
		}

		HSQOBJECT& findTable(const std::string& name, HSQOBJECT& table, SQFUNCTION dlg) {			
			// Check if the table has been referenced
			if(!sq_isnull(table)) {
				return table;
			}
			
			// Find the table
			sq_pushobject(vm, obj);
			sq_pushstring(vm, name.c_str(), name.size());

			if (SQ_FAILED(sq_get(vm, -2))) {
				// Does not exists
				sq_pop(vm, 1);

				// Create one
				sq_newtable(vm);
				sq_getstackobj(vm, -1, &table);
				sq_addref(vm, &table);
				sq_pop(vm, 1);

				sq_pushobject(vm, obj); // Push class obj

				sq_pushstring(vm, name.c_str(), name.size());
				sq_pushobject(vm, table);
				sq_newclosure(vm, dlg, 1);
				if(SQ_FAILED(sq_newslot(vm, -3, false))) {
					throw TypeException("Failed to create table: " + name);
				}

				sq_pop(vm, 1); // Pop class obj

			} else {
				// Return one
				sq_getstackobj(vm, -1, &table);
				sq_addref(vm, &table);
				sq_pop(vm, 2);
			}

			return table;
		}

		struct Tables {
			HSQOBJECT set;
			HSQOBJECT get;
		} tables;
    };

///=============================================================================
/// 
///                                 INSTANCE
///
///=============================================================================
	/**
	* @brief Class that holds instance to a squirrel class object
	*/
	class Instance: public Object {
    public:
		Instance():Object(nullptr) {
			
		}
        Instance(HSQUIRRELVM vm):Object(vm){

        }
        Instance(const Instance& other):Object(other) {
			
		}
        Instance(Instance&& other) NOEXCEPT :Object(std::forward<Instance>(other)) {
        }
        virtual ~Instance(){

		}
		/**
		* @brief Returns the raw class from which this object was created
		*/
		RawClass getClass() const {
			if (isEmpty()) {
				throw TypeException("Instance is null");
			}

			RawClass cls(vm);
			sq_pushobject(vm, obj);
			if(SQ_FAILED(sq_getclass(vm, -1))) {
				throw TypeException("Failed to get class from instance");
			}
			sq_getstackobj(vm, -1, &cls.getObj());
			sq_addref(vm, &cls.getObj());
			sq_pop(vm, 1);
			return cls;
		}
		/**
		* @brief Returns the C++ pointer assigned to this class without performing any checks
		*/
		template<class T>
		T* getAs() const {
			SQUserPointer p;
			sq_pushobject(vm, obj);
			sq_getinstanceup(vm, -1, &p, NULL);
			sq_pop(vm, 1);
			return reinterpret_cast<T*>(p);
        }
        Instance& operator = (const Instance& other){
			Object::operator=(other);
			return *this;
        }
        Instance& operator = (Instance&& other) NOEXCEPT {
            if(this != &other){
                Object::swap(other);
            }
            return *this;
        }
    };

///=============================================================================
/// 
///                                 MACHINE
///
///=============================================================================
	/**
	* @brief Which libraries should be initialized during VM creation?
	*/
	class Stdlib {
    public:
        typedef int Flag;
        static const Flag IO = 0x0001;
        static const Flag BLOB = 0x0002;
        static const Flag MATH = 0x0004;
        static const Flag SYSTEM = 0x0008;
        static const Flag STRING = 0x0010;
        static const Flag ALL = 0xFFFF;
    };

	typedef void(*PrintFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef void(*ErrorFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef SQInteger(*RuntimeErrorFunc)(HSQUIRRELVM);
    typedef void(*CompileErrorFunc)(HSQUIRRELVM v, const SQChar*, const SQChar*, SQInteger, SQInteger);

	/**
	* @brief Squirrel virtual machine
	*/
	class Machine {
	public:
		/**
		* @brief Initializes a squirrel virtual machine with a fixed stack size
		*/
        Machine(int stackSize, Stdlib::Flag flags = 0){
            vm = sq_open(stackSize);
			sq_resetobject(&obj);
            sq_setforeignptr(vm, this);

            registerStdlib(flags);

            setPrintFunc(&Machine::defaultPrintFunc, &Machine::defaultErrorFunc);
            setRuntimeErrorFunc(&Machine::defaultRuntimeErrorFunc);
            setCompileErrorFunc(&Machine::defaultCompilerErrorFunc);

            sq_pushroottable(vm);
            sq_getstackobj(vm,-1,&obj);
            sq_addref(vm, &obj);
            sq_pop(vm, 1);
        }

        Machine(const Machine& other) = delete;
        Machine(Machine&& other) NOEXCEPT {
            swap(other);
			other.vm = nullptr;
			sq_resetobject(&other.obj);
        }

        virtual ~Machine(){
			destroy();
        }
		/**
		* @brief Destroys the virtual machine.
		* @details Note that you need to make sure that all of the squirrel objects
		* currently living on C++ side have to be destroyed before destroying this VM.
		* What are squirrel objects living on C++ side? If you pass any squirrel object via
		* a class or create one (Enum, Table, etc...) they need to be destroyed first.
		*/
		void destroy() {
	        if (vm != nullptr) {
				classMap.clear();
				sq_resetobject(&obj);
                sq_close(vm);
            }
            vm = nullptr;
        }
		/**
		* @brief Registers standard libraries to this virtual machine
		* @details To register only specific libraries, call this function
		* with multiple Stdlib::Flag values connected together via binary OR operator.
		*/
        void registerStdlib(Stdlib::Flag flags){
            sq_pushroottable(vm);
            if(flags & Stdlib::IO)
                sqstd_register_iolib(vm);
            if(flags & Stdlib::BLOB)
                sqstd_register_bloblib(vm);
            if(flags & Stdlib::MATH)
                sqstd_register_mathlib(vm);
            if(flags & Stdlib::SYSTEM)
                sqstd_register_systemlib(vm);
            if(flags & Stdlib::STRING)
                sqstd_register_stringlib(vm);
            sq_pop(vm, 1);
        }
		/**
		* @brief Sets the print function to be assigned as a print() and error()
		* method used in squirrel
		* @code
		* static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        *    va_list vl;
        *    va_start(vl, s);
        *    vprintf(s, vl);
		*	 fprintf(stdout, "\n");
        *    va_end(vl);
		* }
		* static void defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...){
        *    va_list vl;
        *    va_start(vl, s);
        *    vprintf(s, vl);
		*	 fprintf(stderr, "\n");
        *    va_end(vl);
		* }
		* myMachine.setPrintFunc(&defaultPrintFunc, &defaultErrorFunc);
		* @endcode
		*/
        void setPrintFunc(PrintFunc printFunc, ErrorFunc errorFunc){
            sq_setprintfunc(vm, printFunc, errorFunc);
        }
		/**
		* @brief Sets the runtime error function to be used when
		* a runtime error occurs inside of squirrel
		* @details See defaultRuntimeErrorFunc() private static method
		* in the source code of this class for implementation details.
		*/
        void setRuntimeErrorFunc(RuntimeErrorFunc runtimeErrorFunc){
	        sq_newclosure(vm, runtimeErrorFunc, 0);
            sq_seterrorhandler(vm);
        }
		/**
		* @brief Sets the compile error function to be used when
		* an error occurs during script compilation
		* @details See defailtCompileErrorFunc() private static method
		* in the source code of this class for implementation details.
		*/
        void setCompileErrorFunc(CompileErrorFunc compileErrorFunc){
            sq_setcompilererrorhandler(vm, compileErrorFunc);
        }
		/**
		* @brief Swaps the contents of this machine with another one
		*/
        void swap(Machine& other) NOEXCEPT {
            using std::swap;
            swap(vm, other.vm);
            swap(runtimeException, other.runtimeException);
            swap(compileException, other.compileException);
			swap(classMap, other.classMap);
            sq_setforeignptr(vm, this);
            sq_setforeignptr(other.vm, &other);
        }
		/**
		* @brief Returns the Squirrel virtual machine handle associated
		* with this class
		*/
        inline HSQUIRRELVM getHandle() {
            return vm;
        }
		/**
		* @brief Returns the const Squirrel virtual machine handle associated
		* with this class
		*/
        inline const HSQUIRRELVM getHandle() const {
            return vm;
        }
		/**
		* @brief Returns the index of the top slot of the stack
		*/
        inline SQInteger getTop() const {
            return sq_gettop(vm);
        }
		/**
		* @brief Compiles a script from a source file
		* @throws CompileException
		*/
        Script compileFromFile(const std::string path){
            Script script(vm);
            if (SQ_FAILED(sqstd_loadfile(vm, path.c_str(), true))) {
                throw *compileException;
            }

            sq_getstackobj(vm,-1,&script.getObj());
            sq_addref(vm, &script.getObj());
            sq_pop(vm, 1);
            return script;
        }
		/**
		* @brief Compiles a script from a memory
		* @details The script can be associated with a name as a second parameter.
		* This name is used during runtime error information.
		* @throws CompileException
		*/
        Script compileFromData(const std::string& source, const char* name = "buffer"){
            Script script(vm);
            if(SQ_FAILED(sq_compilebuffer(vm, source.c_str(), source.size(), name, true))){
                throw *compileException;
            }

            sq_getstackobj(vm,-1,&script.getObj());
            sq_addref(vm, &script.getObj());
            sq_pop(vm, 1);
            return script;
        }
        /**
		* @brief Runs a script
		* @details When the script runs for the first time, the contens such as
		* class definitions are assigned to the root table (global table).
		* @throws RuntimeException
		*/
        void run(const Script& script) const {
            if(!script.isEmpty()) {
                SQInteger top = sq_gettop(vm);
                sq_pushobject(vm, script.getObj());
                sq_pushroottable(vm);
                SQRESULT result = sq_call(vm, 1, false, true);
                sq_settop(vm, top);
                if(SQ_FAILED(result)){
                    throw *runtimeException;
                }
            }
            else {
                throw RuntimeException("Empty script object");
            }
        }
		/**
		* @brief Calls a member function
		* @param The instance of a class
		* @param Function within the class
		* @param Any number of arguments
		* @throws RuntimeException
		*/
        template<class Ret, class... Args>
        RetVal<Ret> callMember(const Instance& inst, const Function& func, Args&&... args) const {
            static const std::size_t params = sizeof...(Args);

            checkParams(func, params);

            auto top = sq_gettop(vm);
            sq_pushobject(vm, func.getObj());
            sq_pushobject(vm, inst.getObj());

            pushArgs(std::forward<Args>(args)...);

            return callAndReturn<Ret>(func.getNumOfParams(), top);
        }
		/**
		* @brief Calls a global function
		* @param The instance of a class
		* @param Any number of arguments
		* @throws RuntimeException
		*/
		template<class Ret, class... Args>
        RetVal<Ret> callGlobal(const Function& func, Args&&... args) const {
            static const std::size_t params = sizeof...(Args);

            checkParams(func, params);

            auto top = sq_gettop(vm);
            sq_pushobject(vm, func.getObj());
            sq_pushobject(vm, obj);

            pushArgs(std::forward<Args>(args)...);

            return callAndReturn<Ret>(func.getNumOfParams(), top);
        }
		/**
		* @brief Creates a new instance from a class
		* @details Please note that this won't automatically call a constructor
		* within the class. You will have to manually find the constructor via
		* findFunc() method in the RawClass object.
		* @code
		* auto myClass = vm.findClass("MyClass");
		* auto myClassCtor = myClass.findFunc("constructor");
		* auto myInstance = vm.instanceFromClass(myClass);
		* vm.callMember(myInstance, myClassCtor, ... args ...);
		* @endcode
		*/
		Instance instanceFromClass(const RawClass& rawClass) {
			Instance inst(vm);

			auto top = sq_gettop(vm);
			sq_pushobject(vm, rawClass.getObj());
			sq_createinstance(vm, -1);
			sq_remove(vm, -2);
			sq_getstackobj(vm, -1, &inst.getObj());
			sq_addref(vm, &inst.getObj());
			sq_pop(vm, 1);

			return inst;
		}
		/**
		* @brief Adds an enum to global scope
		*/
        Enum addEnum(const std::string& name){
            Enum enm(vm);

            // Register enum to const table
            addConst(name, enm);
            return enm;
		}
		/**
		* @brief Adds a constant variable (as a copy) to global scope
		*/
        template<class T>
        void addConst(const std::string& name, const T& val){
            sq_pushconsttable(vm);
            sq_pushstring(vm, name.c_str(), name.size());
            detail::push<const T&>::f(vm, val);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // Pop const table
        }
		/**
		* @brief Adds a class to global scope
		* @code
		* // Bind MyCppClass class to squirrel using std::function
		* // that accepts two integers.
		* auto myClass = myMachine.addClass<MyCppClass>("MyCppClass", std::function<MyCppClass*(int, int)>([](int a, int b){
		*     return new MyCppClass(a, b);
		* }));
		* // Bind MyCppClass class to squirrel, but it will never be deleted withing squirrel
		* // You will have to keep track of the instance and delete it manually
		* // withing C++
		* std::unique_ptr<MyCppClass> ptr;
		* auto myClass = myMachine.addClass<MyCppClass>("MyCppClass", std::function<MyCppClass*(int, int)>([](int a, int b){
		*     ptr.reset(new MyCppClass(a, b));
		*     return ptr.get();
		* }), false); // Should be released when instance falls out of scope?
		* @endcode
		*/
		template<class T, class... Args>
        Class<T> addClass(const std::string& name, const std::function<T*(Args...)>& allocator = std::bind(&detail::defaultClassAllocator<T>), bool release = true){
            sq_pushroottable(vm);
			Class<T> cls(vm);
            detail::bindClass(vm, name, allocator, cls.getObj(), release);
            sq_pop(vm, 1);
			return cls;
        }
		/**
		* @brief Adds a class to global scope
		* @code
		* // Bind MyCppClass class to squirrel using MyCppClass constructor
		* // that accepts two integers.
		* auto myClass = myMachine.addClass<MyCppClass>("MyCppClass", ssq::Constructor<MyCppClass(int,int)>());
		* @endcode
		*/
		template<class T, class... Args>
        Class<T> addClass(const std::string& name, const Constructor<T(Args...)>& constructor, bool release = true){
            sq_pushroottable(vm);
			Class<T> cls(vm);
			const std::function<T*(Args...)> func = &constructor.allocate;
            detail::bindClass(vm, name, func, cls.getObj(), release);
            sq_pop(vm, 1);
			return cls;
        }
		/**
		* @brief Adds an abstract class to squirrel
		* @details Abstract classes cannot be allocated or released. This may be 
		* usefull when binding base C++ classes that contain pure virtual functions.
		* @code
		* // Bind MyCppClass class to squirrel
		* auto myAbstractClass = myMachine.addAbstractClass<MyAbstractCppClass>("MyAbstractCppClass");
		* @endcode
		*/
		template<class T, class... Args>
        Class<T> addAbstractClass(const std::string& name){
            sq_pushroottable(vm);
			Class<T> cls(vm);
            detail::bindAbstractClass<T>(vm, name, cls.getObj());
            sq_pop(vm, 1);
			return cls;
        }
		/**
		* @brief Creates a new instance of squirrel table
		*/
		Table newTable() {
			return Table(vm);
        }
		/**
		* @brief Finds a function from global scope
		* @throws NotFoundException if not found and TypeException if the name
		* is not associated with a function
		*/
        Function findFunc(const std::string& name) const {
            sq_pushroottable(vm);
            sq_pushstring(vm, name.c_str(), name.size());

            if(SQ_FAILED(sq_get(vm, -2))) {
                sq_pop(vm, 1);
                throw NotFoundException("Function not found");
            }

            SQObjectType value_type = sq_gettype(vm, -1);
            if (value_type != OT_CLOSURE && value_type != OT_NATIVECLOSURE) {
                sq_pop(vm, 2);
                throw TypeException("Not a function");
            }

            SQUnsignedInteger nparams;
            SQUnsignedInteger nfreevars;
            if (SQ_FAILED(sq_getclosureinfo(vm, -1, &nparams, &nfreevars))) {
                sq_pop(vm, 2);
                throw TypeException("Get function info failed");
            }

            Function func(vm, nparams -1);
            sq_getstackobj(vm, -1, &func.getObj());
            sq_addref(vm, &func.getObj());
            sq_pop(vm, 2);

            return func;
        }   
		/**
		* @brief Finds a class from global scope
		* @throws NotFoundException if not found and TypeException if the name
		* is not associated with a class
		*/
		RawClass findClass(const std::string& name) const {
			sq_pushroottable(vm);
            sq_pushstring(vm, name.c_str(), name.size());

            if(SQ_FAILED(sq_get(vm, -2))) {
                sq_pop(vm, 1);
                throw NotFoundException("Class not found");
            }

            SQObjectType value_type = sq_gettype(vm, -1);
            if (value_type != OT_CLASS) {
                sq_pop(vm, 2);
                throw TypeException("Not a class");
            }

			RawClass klass(vm);
			sq_getstackobj(vm, -1, &klass.getObj());
            sq_addref(vm, &klass.getObj());
            sq_pop(vm, 2);

            return klass;
		}
		/**
		* @brief Adds a function to global scope
		* @code
		* myMachine.addFunc("myCustomSqrt", std::function<float(float)>([](float val){
        *     return sqrt(val);
		* });
		* @endcode
		*/
		template<class Ret, class... Args>
		void addFunc(const std::string& name, const std::function<Ret(Args...)>& func){
            sq_pushroottable(vm);
            detail::bindFunc(vm, name, func);
            sq_pop(vm, 1);
        }
		/**
		* @brief Adds a function to global scope
		* @code
		* myMachine.addFunc<float>("myCustomSqrt", [](float val){
        *     return sqrt(val);
		* });
		* @endcode
		*/
		template<class Ret, class... Args, typename F>
		void addFunc(const std::string& name, F&& lambda){
			sq_pushroottable(vm);
            detail::bindFunc(vm, name, std::function<Ret(Args...)>(lambda));
            sq_pop(vm, 1);
        }
		/**
		* @brief Adds a function to global scope
		* @code
		* static float myCustomSqrt(float val){
        *     return sqrt(val);
		* }
		* myMachine.addFunc<float>("myCustomSqrt", &myCustomSqrt);
		* @endcode
		*/
		template<class Ret, class... Args>
		void addFunc(const std::string& name, Ret (*func)(Args...)){
			sq_pushroottable(vm);
            detail::bindFunc(vm, name, std::function<Ret(Args...)>(func));
            sq_pop(vm, 1);
        }

        Machine& operator = (const Machine& other) = delete;
        Machine& operator = (Machine&& other) NOEXCEPT {
            if(this != &other){
                swap(other);
            }
            return *this;
        }

		template<class T>
		const HSQOBJECT& getClassObject() const {
	        return classMap.at(typeid(T).hash_code());
        }

		template<class T>
		HSQOBJECT& getClassObjectOrCreate() {
	        return classMap[typeid(T).hash_code()];
        }

    protected:
        std::unique_ptr<CompileException> compileException;
        std::unique_ptr<RuntimeException> runtimeException;
		HSQUIRRELVM vm;
		HSQOBJECT obj;
		detail::ClassMap classMap;

    private:
		static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...){
            va_list vl;
            va_start(vl, s);
            vprintf(s, vl);
			printf("\n");
            va_end(vl);
        }

        static void defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...){
            va_list vl;
            va_start(vl, s);
            fprintf(stderr, s, vl);
			fprintf(stderr, "\n");
            va_end(vl);
        }

        static SQInteger defaultRuntimeErrorFunc(HSQUIRRELVM vm) {
            SQStackInfos si;
            sq_stackinfos(vm, 1, &si);

            auto source = (si.source != nullptr ? si.source : "null");
            auto funcname = (si.funcname != nullptr ? si.funcname : "unknown");

            const SQChar *sErr = 0;
            if(sq_gettop(vm) >= 1){
                if(SQ_FAILED(sq_getstring(vm, 2, &sErr))){
                    sErr = "unknown error";
                }
            }

            auto ptr = reinterpret_cast<Machine*>(sq_getforeignptr(vm));
            ptr->runtimeException.reset(new RuntimeException(
                sErr,
                source,
                funcname,
                si.line
            ));
            return 0;
        }

        static void defaultCompilerErrorFunc(
            HSQUIRRELVM vm,
            const SQChar* desc,
            const SQChar* source,
            SQInteger line,
            SQInteger column) {
            Machine* ptr = reinterpret_cast<Machine*>(sq_getforeignptr(vm));
            ptr->compileException.reset(new CompileException(
                desc,
                source,
                line,
                column
            ));
        }

        static inline void checkParams(const Function& func, int params) {
            if(func.getNumOfParams() != params){
                throw TypeException("Number of arguments does not match");
            }
        }
        template<class Ret>
        inline RetVal<Ret> callAndReturn(SQUnsignedInteger nparams, SQInteger top) const {
            if(SQ_FAILED(sq_call(vm, 1 + nparams, true, true))){
                sq_settop(vm, top);
				if (runtimeException == nullptr)
					throw RuntimeException("Unknown squirrel runtime error");
                throw *runtimeException;
            }
            
            RetVal<Ret> ret(vm, -1);
            sq_settop(vm, top);
            return ret;
        }

        template <class First, class... Rest> 
        void pushArgs(First&& first, Rest&&... rest) const {
            detail::push<First>::f(vm, first);
            pushArgs(std::forward<Rest>(rest)...);
        }
    };

///=============================================================================
/// 
///                      INTERNAL IMPLEMENTATION
///
///=============================================================================
	template<class T>
	inline const HSQOBJECT& detail::getClassObject(HSQUIRRELVM vm) {
		Machine* machine = reinterpret_cast<Machine*>(sq_getforeignptr(vm));
		try {
			return machine->getClassObject<T>();
		} catch (std::out_of_range& e) {
			(void)e;
			throw TypeException(std::string(typeid(T).name()) + " not registered in this VM");
		}
	}

	template<class T>
	inline HSQOBJECT& detail::getClassObjectOrCreate(HSQUIRRELVM vm) {
		Machine* machine = reinterpret_cast<Machine*>(sq_getforeignptr(vm));
		return machine->getClassObjectOrCreate<T>();
	}

///=============================================================================
/// 
///                      INTERNAL PARAM TYPES
///
///=============================================================================
	template<> struct detail::ParamType<int8_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<int16_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<int32_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<int64_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<uint8_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<uint16_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<uint32_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<uint64_t> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<float> {
		static const char value = 'f';
	};

	template<> struct detail::ParamType<double> {
		static const char value = 'f';
	};

	template<> struct detail::ParamType<bool> {
		static const char value = 'b';
	};

	template<> struct detail::ParamType<void*> {
		static const char value = 'p';
	};

	template<> struct detail::ParamType<RawClass> {
		static const char value = 'y';
	};

	template<> struct detail::ParamType<Instance> {
		static const char value = 'x';
	};

	template<> struct detail::ParamType<Table> {
		static const char value = 't';
	};

	template<> struct detail::ParamType<std::string> {
		static const char value = 's';
	};

	template<> struct detail::ParamType<std::wstring> {
		static const char value = 's';
	};

	template<> struct detail::ParamType<const int8_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const int16_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const int32_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const int64_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const uint8_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const uint16_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const uint32_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const uint64_t&> {
		static const char value = 'i';
	};

	template<> struct detail::ParamType<const float&> {
		static const char value = 'f';
	};

	template<> struct detail::ParamType<const double&> {
		static const char value = 'f';
	};

	template<> struct detail::ParamType<const bool&> {
		static const char value = 'b';
	};

	template<> struct detail::ParamType<const void*&> {
		static const char value = 'p';
	};

	template<> struct detail::ParamType<const void*> {
		static const char value = 'p';
	};

	template<> struct detail::ParamType<const std::string&> {
		static const char value = 's';
	};

	template<> struct detail::ParamType<const std::wstring&> {
		static const char value = 's';
	};

	template<> struct detail::ParamType<const RawClass&> {
		static const char value = 'y';
	};

	template<> struct detail::ParamType<const Instance&> {
		static const char value = 'x';
	};

	template<> struct detail::ParamType<const Table&> {
		static const char value = 't';
	};

///=============================================================================
/// 
///                      INTERNAL POP FUNCTIONS
///
///=============================================================================
	template<>
    inline int8_t detail::pop<int8_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int8_t from squirrel stack");
        return static_cast<int8_t>(val);
    }

	template<>
    inline int16_t detail::pop<int16_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int16_t from squirrel stack");
        return static_cast<int16_t>(val);
    }

	template<>
    inline int32_t detail::pop<int32_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int32_t from squirrel stack");
        return static_cast<int32_t>(val);
    }

	template<>
    inline int64_t detail::pop<int64_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int64_t from squirrel stack");
        return static_cast<int64_t>(val);
    }

	template<>
    inline uint8_t detail::pop<uint8_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint8_t from squirrel stack");
        return static_cast<uint8_t>(val);
    }

	template<>
    inline uint16_t detail::pop<uint16_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint16_t from squirrel stack");
        return static_cast<uint16_t>(val);
    }

	template<>
    inline uint32_t detail::pop<uint32_t>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint32_t from squirrel stack");
        return static_cast<uint32_t>(val);
    }

	template<>
    inline uint64_t detail::pop<uint64_t>::f(HSQUIRRELVM vm, int index){
        uint64_t val = 0;
        if (SQ_FAILED(sq_getinteger(vm, index, reinterpret_cast<SQInteger*>(&val))))
            throw TypeException("Could not get uint64_t from squirrel stack");
        return val;
    }

    template<>
    inline float detail::pop<float>::f(HSQUIRRELVM vm, int index){
        SQFloat val;
        if (SQ_FAILED(sq_getfloat(vm, index, &val)))
            throw TypeException("Could not get float from squirrel stack");
        return static_cast<float>(val);
    }

	template<>
    inline double detail::pop<double>::f(HSQUIRRELVM vm, int index){
        SQFloat val;
        if (SQ_FAILED(sq_getfloat(vm, index, &val)))
            throw TypeException("Could not get double from squirrel stack");
        return static_cast<double>(val);
    }

    template<>
    inline std::string detail::pop<std::string>::f(HSQUIRRELVM vm, int index){
        const SQChar* val = nullptr;
        if (SQ_FAILED(sq_getstring(vm, index, &val)))
            throw TypeException("Could not get std::string from squirrel stack");
        return std::string(val);
    }

	template<>
    inline std::wstring detail::pop<std::wstring>::f(HSQUIRRELVM vm, int index){
        const SQChar* val = nullptr;
        if (SQ_FAILED(sq_getstring(vm, index, &val)))
            throw TypeException("Could not get std::wstring from squirrel stack");
        return SIMPLESQ_UTF8_TO_WSTR(val);
    }

    template<>
    inline bool detail::pop<bool>::f(HSQUIRRELVM vm, int index){
        SQBool val;
        if (SQ_FAILED(sq_getbool(vm, index, &val)))
            throw TypeException("Could not get bool from squirrel stack");
        return static_cast<bool>(val);
    }

	template<>
    inline SQUserPointer detail::pop<SQUserPointer>::f(HSQUIRRELVM vm, int index){
        SQUserPointer val;
        if (SQ_FAILED(sq_getuserpointer(vm, index, &val)))
            throw TypeException("Could not get user pointer from squirrel stack 1");
        return val;
    }

	template<>
    inline RawClass detail::pop<RawClass>::f(HSQUIRRELVM vm, int index){
        RawClass val(vm);
		if(sq_gettype(vm, index) != OT_CLASS || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get class object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }

	template<>
    inline Instance detail::pop<Instance>::f(HSQUIRRELVM vm, int index){
        Instance val(vm);
		if(sq_gettype(vm, index) != OT_INSTANCE || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get instance object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }

	template<>
    inline Function detail::pop<Function>::f(HSQUIRRELVM vm, int index){
		auto type = sq_gettype(vm, index);
		if(type != OT_CLOSURE && type != OT_NATIVECLOSURE) {
			throw TypeException("Could not get function object from squirrel stack, object not a closure!");
		}

		SQUnsignedInteger nparams, nfreevars;
		if(SQ_FAILED(sq_getclosureinfo(vm, index, &nparams, &nfreevars))) {
			throw TypeException("Could not get function info from squirrel stack");
		}

		Function func(vm, nparams -1);

		if(SQ_FAILED(sq_getstackobj(vm, index, &func.getObj()))) {
			throw TypeException("Could not get function object from squirrel stack");
		}
		sq_addref(vm, &func.getObj());
		return func;
    }

	template<>
    inline Object detail::pop<Object>::f(HSQUIRRELVM vm, int index){
		Object func(vm);

		if(SQ_FAILED(sq_getstackobj(vm, index, &func.getObj()))) {
			throw TypeException("Could not get object from squirrel stack");
		}
		sq_addref(vm, &func.getObj());
		return func;
    }

	template<>
    inline Table detail::pop<Table>::f(HSQUIRRELVM vm, int index){
        Table val(vm, false);
		if(sq_gettype(vm, index) != OT_TABLE || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get table object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }

///=============================================================================
/// 
///                 INTERNAL POP FUNCTIONS CONST REFERENCE
///
///=============================================================================
	template<>
    inline int8_t detail::pop<const int8_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int8_t from squirrel stack");
        return static_cast<int8_t>(val);
    }

	template<>
    inline int16_t detail::pop<const int16_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int16_t from squirrel stack");
        return static_cast<int16_t>(val);
    }

	template<>
    inline int32_t detail::pop<const int32_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int32_t from squirrel stack");
        return static_cast<int32_t>(val);
    }

	template<>
    inline int64_t detail::pop<const int64_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get int64_t from squirrel stack");
        return static_cast<int64_t>(val);
    }

	template<>
    inline uint8_t detail::pop<const uint8_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint8_t from squirrel stack");
        return static_cast<uint8_t>(val);
    }

	template<>
    inline uint16_t detail::pop<const uint16_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint16_t from squirrel stack");
        return static_cast<uint16_t>(val);
    }

	template<>
    inline uint32_t detail::pop<const uint32_t&>::f(HSQUIRRELVM vm, int index){
        SQInteger val;
        if (SQ_FAILED(sq_getinteger(vm, index, &val)))
            throw TypeException("Could not get uint32_t from squirrel stack");
        return static_cast<uint32_t>(val);
    }

	template<>
    inline uint64_t detail::pop<const uint64_t&>::f(HSQUIRRELVM vm, int index){
		uint64_t val = 0;
        if (SQ_FAILED(sq_getinteger(vm, index, reinterpret_cast<SQInteger*>(&val))))
            throw TypeException("Could not get uint64_t from squirrel stack");
        return val;
    }

    template<>
    inline float detail::pop<const float&>::f(HSQUIRRELVM vm, int index){
        SQFloat val;
        if (SQ_FAILED(sq_getfloat(vm, index, &val)))
            throw TypeException("Could not get float from squirrel stack");
        return static_cast<float>(val);
    }

	template<>
    inline double detail::pop<const double&>::f(HSQUIRRELVM vm, int index){
        SQFloat val;
        if (SQ_FAILED(sq_getfloat(vm, index, &val)))
            throw TypeException("Could not get double from squirrel stack");
        return static_cast<double>(val);
    }

    template<>
    inline std::string detail::pop<const std::string&>::f(HSQUIRRELVM vm, int index){
        const SQChar* val = nullptr;
        if (SQ_FAILED(sq_getstring(vm, index, &val)))
            throw TypeException("Could not get std::string from squirrel stack");
        return std::string(val);
    }

	template<>
    inline std::wstring detail::pop<const std::wstring&>::f(HSQUIRRELVM vm, int index){
        const SQChar* val = nullptr;
        if (SQ_FAILED(sq_getstring(vm, index, &val)))
            throw TypeException("Could not get std::wstring from squirrel stack");
        return SIMPLESQ_UTF8_TO_WSTR(val);
    }

    template<>
    inline bool detail::pop<const bool&>::f(HSQUIRRELVM vm, int index){
        SQBool val;
        if (SQ_FAILED(sq_getbool(vm, index, &val)))
            throw TypeException("Could not get bool from squirrel stack");
        return static_cast<bool>(val);
    }

	template<>
    inline SQUserPointer detail::pop<const SQUserPointer&>::f(HSQUIRRELVM vm, int index){
        SQUserPointer val;
        if (SQ_FAILED(sq_getuserpointer(vm, index, &val)))
            throw TypeException("Could not get user pointer from squirrel stack");
		return val;
    }

	template<>
    inline RawClass detail::pop<const RawClass&>::f(HSQUIRRELVM vm, int index){
        RawClass val(vm);
		if(sq_gettype(vm, index) != OT_CLASS || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get class object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }

	template<>
    inline Instance detail::pop<const Instance&>::f(HSQUIRRELVM vm, int index){
        Instance val(vm);
		if(sq_gettype(vm, index) != OT_INSTANCE || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get class object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }

	template<>
    inline Function detail::pop<const Function&>::f(HSQUIRRELVM vm, int index){
		auto type = sq_gettype(vm, index);
		if(type != OT_CLOSURE && type != OT_NATIVECLOSURE) {
			throw TypeException("Could not get function object from squirrel stack, object not a closure!");
		}

		SQUnsignedInteger nparams, nfreevars;
		if(SQ_FAILED(sq_getclosureinfo(vm, index, &nparams, &nfreevars))) {
			throw TypeException("Could not get function info from squirrel stack");
		}

		Function func(vm, nparams -1);

		if(SQ_FAILED(sq_getstackobj(vm, index, &func.getObj()))) {
			throw TypeException("Could not get function object from squirrel stack");
		}
		sq_addref(vm, &func.getObj());
		return func;
    }

	template<>
    inline Object detail::pop<const Object&>::f(HSQUIRRELVM vm, int index){
		Object func(vm);

		if(SQ_FAILED(sq_getstackobj(vm, index, &func.getObj()))) {
			throw TypeException("Could not get object from squirrel stack");
		}
		sq_addref(vm, &func.getObj());
		return func;
    }

	template<>
    inline Table detail::pop<const Table&>::f(HSQUIRRELVM vm, int index){
        Table val(vm, false);
		if(sq_gettype(vm, index) != OT_TABLE || SQ_FAILED(sq_getstackobj(vm, index, &val.getObj())))
			throw TypeException("Could not get table object from squirrel stack");
		sq_addref(vm, &val.getObj());
		return val;
    }
}
#endif