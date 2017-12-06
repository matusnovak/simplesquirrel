#pragma once
#ifndef SQUIRREL_BIND_VM_HEADER_H
#define SQUIRREL_BIND_VM_HEADER_H

#include "exceptions.hpp"
#include "table.hpp"
#include "script.hpp"
#include "args.hpp"
#include "class.hpp"
#include "instance.hpp"

#include <memory>

namespace SquirrelBind {
	typedef void(*SqPrintFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef void(*SqErrorFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef SQInteger(*SqRuntimeErrorFunc)(HSQUIRRELVM);
    typedef void(*SqCompileErrorFunc)(HSQUIRRELVM v, const SQChar*, const SQChar*, SQInteger, SQInteger);

	class SqLibs {
    public:
        typedef int Flag;
		static const Flag NONE = 0x0000;
        static const Flag IO = 0x0001;
        static const Flag BLOB = 0x0002;
        static const Flag MATH = 0x0004;
        static const Flag SYSTEM = 0x0008;
        static const Flag STRING = 0x0010;
        static const Flag ALL = 0xFFFF;
    };

	class SqVM: public SqTable {
	public:
		SqVM(size_t stackSize, SqLibs::Flag flags = 0x00);

		void destroy();

		virtual ~SqVM();

		void swap(SqVM& other) NOEXCEPT;

		SqVM(const SqVM& other) = delete;
		
		SqVM(SqVM&& other) NOEXCEPT;

		void registerStdlib(SqLibs::Flag flags);

		void setPrintFunc(SqPrintFunc printFunc, SqErrorFunc errorFunc);

		void setRuntimeErrorFunc(SqRuntimeErrorFunc runtimeErrorFunc);

		void setCompileErrorFunc(SqCompileErrorFunc compileErrorFunc);
		/**
		* @brief Returns the index of the top slot of the stack
		*/
		SQInteger getTop() const;

		const SqCompileException& getLastCompileException() const {
			return *compileException.get();
		}

		const SqRuntimeException& getLastRuntimeException() const {
			return *runtimeException.get();
		}
		/**
		* @brief Compiles a script from a memory
		* @details The script can be associated with a name as a second parameter.
		* This name is used during runtime error information.
		* @throws CompileException
		*/
		SqScript compileSource(const char* source, const char* name = "buffer");
		/**
		* @brief Compiles a script from a source file
		* @throws CompileException
		*/
		SqScript compileFile(const char* path);

		/**
		* @brief Runs a script
		* @details When the script runs for the first time, the contens such as
		* class definitions are assigned to the root table (global table).
		* @throws RuntimeException
		*/
		void run(const SqScript& script) const;

		/**
		* @brief Calls a global function
		* @param func The instance of a function
		* @param args Any number of arguments
		* @throws RuntimeException
		*/
		template<class... Args>
        SqObject callFunc(const SqFunction& func, const SqObject& env, Args&&... args) const {
            static const std::size_t params = sizeof...(Args);

            if(func.getNumOfParams() != params){
                throw SqRuntimeException("Number of arguments does not match");
            }

            auto top = sq_gettop(vm);
            sq_pushobject(vm, func.get());
            sq_pushobject(vm, env.get());

            pushArgs(std::forward<Args>(args)...);

			return callAndReturn(params, top);
        }
		/**
		* @brief Creates a new instance of class and call constructor with given arguments
		* @param cls The object of a class
		* @param args Any number of arguments
		* @throws RuntimeException
		*/
		template<class... Args>
		SqInstance newInstance(const SqClass& cls, Args&&... args) const {
			SqInstance inst = newInstanceNoCtor(cls);
			SqFunction ctor = cls.findFunc("constructor");
			callFunc(ctor, inst, std::forward<Args>(args)...);
			return inst;
		}
		/**
		* @brief Creates a new instance of class without calling a constructor
		* @param cls The object of a class
		* @throws RuntimeException
		*/
		SqInstance newInstanceNoCtor(const SqClass& cls) const {
			SqInstance inst(vm);

			sq_pushobject(vm, cls.get());
			sq_createinstance(vm, -1);
			sq_remove(vm, -2);
			sq_getstackobj(vm, -1, &inst.get());
			sq_addref(vm, &inst.get());
			sq_pop(vm, 1);

			return inst;
		}

		void debugStack() const;

		template<class T>
		HSQOBJECT& getClassObjectOrCreate() {
	        return classMap[typeid(T).hash_code()];
        }

		SqVM& operator = (const SqVM& other) = delete;

		SqVM& operator = (SqVM&& other) NOEXCEPT;
	private:
		std::unique_ptr<SqCompileException> compileException;
        std::unique_ptr<SqRuntimeException> runtimeException;
		ClassMap classMap;

		static void pushArgs();

        template <class First, class... Rest> 
        void pushArgs(First&& first, Rest&&... rest) const {
            detail::push<First>::f(vm, first);
            pushArgs(std::forward<Rest>(rest)...);
        }

		SqObject callAndReturn(SQUnsignedInteger nparams, SQInteger top) const;

		static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);

		static void defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...);

		static SQInteger defaultRuntimeErrorFunc(HSQUIRRELVM vm);

		static void defaultCompilerErrorFunc(HSQUIRRELVM vm, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column);
	};

	namespace detail {
		template<class T>
		inline HSQOBJECT& detail::getClassObjectOrCreate(HSQUIRRELVM vm) {
			SqVM* machine = reinterpret_cast<SqVM*>(sq_getforeignptr(vm));
			return machine->getClassObjectOrCreate<T>();
		}
	}
}

#endif