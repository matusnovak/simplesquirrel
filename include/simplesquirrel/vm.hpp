#pragma once
#ifndef SSQ_VM_HEADER_H
#define SSQ_VM_HEADER_H

#include "exceptions.hpp"
#include "table.hpp"
#include "script.hpp"
#include "args.hpp"
#include "class.hpp"
#include "instance.hpp"
#include "function.hpp"
#include "array.hpp"

#include <memory>

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable: 4251 )
#endif

namespace ssq {
    /**
     * @ingroup simplesquirrel
     */
    typedef void(*SqPrintFunc)(HSQUIRRELVM, const SQChar*, ...);
    /**
     * @ingroup simplesquirrel
     */
    typedef void(*SqErrorFunc)(HSQUIRRELVM, const SQChar*, ...);
    /**
     * @ingroup simplesquirrel
     */
    typedef SQInteger(*SqRuntimeErrorFunc)(HSQUIRRELVM);
    /**
     * @ingroup simplesquirrel
     */
    typedef void(*SqCompileErrorFunc)(HSQUIRRELVM, const SQChar*, const SQChar*, SQInteger, SQInteger);
    /**
     * @ingroup simplesquirrel
     */
    class Libs {
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

    /**
    * @brief Squirrel Virtual Machine object
    * @ingroup simplesquirrel
    */
    class SSQ_API VM: public Table {
    public:
        /**
        * @brief Creates a VM with a fixed stack size
        */
        VM(size_t stackSize, Libs::Flag flags = 0x00);
        /**
        * @brief Destroys the VM and all of this objects
        */
        void destroy();
        /**
        * @brief Destructor
        */
        virtual ~VM();
        /**
        * @brief Swaps the contents of this VM with another one
        */
        void swap(VM& other) NOEXCEPT;
        /**
        * @brief Disabled copy constructor
        */
        VM(const VM& other) = delete;
        /**
        * @brief Move constructor
        */
        VM(VM&& other) NOEXCEPT;
        /**
        * @brief Registers standard template libraries
        */
        void registerStdlib(Libs::Flag flags);
        /**
        * @brief Registers print and error functions
        */
        void setPrintFunc(SqPrintFunc printFunc, SqErrorFunc errorFunc);
        /**
        * @brief Registers runtime error function
        */
        void setRuntimeErrorFunc(SqRuntimeErrorFunc runtimeErrorFunc);
        /**
        * @brief Registers compilation error function
        */
        void setCompileErrorFunc(SqCompileErrorFunc compileErrorFunc);
        /**
        * @brief Returns the index of the top slot of the stack
        */
        SQInteger getTop() const;
        /**
        * @brief Returns the last compilation exception
        */
        const CompileException& getLastCompileException() const {
            return *compileException.get();
        }
        /**
        * @brief Returns the last runtime exception
        */
        const RuntimeException& getLastRuntimeException() const {
            return *runtimeException.get();
        }
        /**
        * @brief Compiles a script from a memory
        * @details The script can be associated with a name as a second parameter.
        * This name is used during runtime error information.
        * @throws CompileException
        */
        Script compileSource(const char* source, const char* name = "buffer");
        /**
        * @brief Compiles a script from a source file
        * @throws CompileException
        */
        Script compileFile(const char* path);
        /**
        * @brief Runs a script
        * @details When the script runs for the first time, the contens such as
        * class definitions are assigned to the root table (global table).
        * @throws RuntimeException
        */
        void run(const Script& script) const;
        /**
        * @brief Calls a global function
        * @param func The instance of a function
        * @param args Any number of arguments
        * @throws RuntimeException if an exception is thrown or number of arguments
        * do not match
        * @throws TypeException if casting from Squirrel objects to C++ objects failed
        */
        template<class... Args>
        Object callFunc(const Function& func, const Object& env, Args&&... args) const {
            static const std::size_t params = sizeof...(Args);

            if(func.getNumOfParams() != params){
                throw RuntimeException("Number of arguments does not match");
            }

            auto top = sq_gettop(vm);
            sq_pushobject(vm, func.getRaw());
            sq_pushobject(vm, env.getRaw());

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
        Instance newInstance(const Class& cls, Args&&... args) const {
            Instance inst = newInstanceNoCtor(cls);
            Function ctor = cls.findFunc("constructor");
            callFunc(ctor, inst, std::forward<Args>(args)...);
            return inst;
        }
        /**
        * @brief Creates a new instance of class without calling a constructor
        * @param cls The object of a class
        * @throws RuntimeException
        */
        Instance newInstanceNoCtor(const Class& cls) const {
            Instance inst(vm);
            sq_pushobject(vm, cls.getRaw());
            sq_createinstance(vm, -1);
            sq_remove(vm, -2);
            sq_getstackobj(vm, -1, &inst.getRaw());
            sq_addref(vm, &inst.getRaw());
            sq_pop(vm, 1);
            return inst;
        }
        /**
        * @brief Creates a new empty table
        */
        Table newTable() const {
            return Table(vm);
        }
        /**
        * @brief Creates a new empty array
        */
        Array newArray() const {
            return Array(vm);
        }
        /**
        * @brief Creates a new array
        */
        template<class T>
        Array newArray(const std::vector<T>& vector) const {
            return Array(vm, vector);
        }
        /**
         * @brief Adds a new enum to this table
         */
        Enum addEnum(const char* name);
        /**
         * @brief Adds a new constant key-value pair to this table
         */
        template<typename T>
        inline void setConst(const char* name, const T& value) {
            sq_pushconsttable(vm);
            sq_pushstring(vm, name, strlen(name));
            detail::push<T>(vm, value);
            sq_newslot(vm, -3, false);
            sq_pop(vm,1); // pop table
        }
        /**
        * @brief Prints stack objects
        */
        void debugStack() const;
		/**
        * @brief Add registered class object into the table of known classes
        */
		void addClassObj(size_t hashCode, const HSQOBJECT& obj);
		/**
        * @brief Get registered class object from hash code
        */
		const HSQOBJECT& getClassObj(size_t hashCode);
        /**
        * @brief Copy assingment operator
        */
        VM& operator = (const VM& other) = delete;
        /**
        * @brief Move assingment operator
        */
        VM& operator = (VM&& other) NOEXCEPT;
    private:
        std::unique_ptr<CompileException> compileException;
        std::unique_ptr<RuntimeException> runtimeException;
		std::unordered_map<size_t, HSQOBJECT> classMap;

        static void pushArgs();

        template <class First, class... Rest> 
        void pushArgs(First&& first, Rest&&... rest) const {
            detail::push(vm, first);
            pushArgs(std::forward<Rest>(rest)...);
        }

        Object callAndReturn(SQUnsignedInteger nparams, SQInteger top) const;

        static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);

        static void defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...);

        static SQInteger defaultRuntimeErrorFunc(HSQUIRRELVM vm);

        static void defaultCompilerErrorFunc(HSQUIRRELVM vm, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column);
    };
}

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif