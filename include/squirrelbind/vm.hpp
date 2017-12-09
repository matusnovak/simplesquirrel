#pragma once
#ifndef SQUIRREL_BIND_VM_HEADER_H
#define SQUIRREL_BIND_VM_HEADER_H

#include "exceptions.hpp"
#include "table.hpp"
#include "script.hpp"
#include "args.hpp"
#include "class.hpp"
#include "instance.hpp"
#include "function.hpp"
#include "array.hpp"

#include <memory>

namespace SquirrelBind {
    typedef void(*SqPrintFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef void(*SqErrorFunc)(HSQUIRRELVM, const SQChar*, ...);
    typedef SQInteger(*SqRuntimeErrorFunc)(HSQUIRRELVM);
    typedef void(*SqCompileErrorFunc)(HSQUIRRELVM, const SQChar*, const SQChar*, SQInteger, SQInteger);

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

    /**
    * @brief Squirrel Virtual Machine object
    */
    class SqVM: public SqTable {
    public:
        /**
        * @brief Creates a VM with a fixed stack size
        */
        SqVM(size_t stackSize, SqLibs::Flag flags = 0x00);
        /**
        * @brief Destroys the VM and all of this objects
        */
        void destroy();
        /**
        * @brief Destructor
        */
        virtual ~SqVM();
        /**
        * @brief Swaps the contents of this VM with another one
        */
        void swap(SqVM& other) NOEXCEPT;
        /**
        * @brief Disabled copy constructor
        */
        SqVM(const SqVM& other) = delete;
        /**
        * @brief Move constructor
        */
        SqVM(SqVM&& other) NOEXCEPT;
        /**
        * @brief Registers standard template libraries
        */
        void registerStdlib(SqLibs::Flag flags);
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
        const SqCompileException& getLastCompileException() const {
            return *compileException.get();
        }
        /**
        * @brief Returns the last runtime exception
        */
        const SqRuntimeException& getLastRuntimeException() const {
            return *runtimeException.get();
        }
        /**
        * @brief Compiles a script from a memory
        * @details The script can be associated with a name as a second parameter.
        * This name is used during runtime error information.
        * @throws SqCompileException
        */
        SqScript compileSource(const char* source, const char* name = "buffer");
        /**
        * @brief Compiles a script from a source file
        * @throws SqCompileException
        */
        SqScript compileFile(const char* path);
        /**
        * @brief Runs a script
        * @details When the script runs for the first time, the contens such as
        * class definitions are assigned to the root table (global table).
        * @throws SqRuntimeException
        */
        void run(const SqScript& script) const;
        /**
        * @brief Calls a global function
        * @param func The instance of a function
        * @param args Any number of arguments
        * @throws SqRuntimeException if an exception is thrown or number of arguments
        * do not match
        * @throws SqTypeException if casting from Squirrel objects to C++ objects failed
        */
        template<class... Args>
        SqObject callFunc(const SqFunction& func, const SqObject& env, Args&&... args) const {
            static const std::size_t params = sizeof...(Args);

            if(func.getNumOfParams() != params){
                throw SqRuntimeException("Number of arguments does not match");
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
        * @throws SqRuntimeException
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
        * @throws SqRuntimeException
        */
        SqInstance newInstanceNoCtor(const SqClass& cls) const {
            SqInstance inst(vm);
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
        SqTable newTable() const {
            return SqTable(vm);
        }
        /**
        * @brief Creates a new empty array
        */
        SqArray newArray() const {
            return SqArray(vm);
        }
        /**
        * @brief Creates a new array
        */
        template<class T>
        SqArray newArray(const std::vector<T>& vector) const {
            return SqArray(vm, vector);
        }
        /**
        * @brief Exposes a derived class of SqClassWrapper to this VM
        */
        template<class T>
        void expose(){
            T::sqExposeClass(*this);
        }
        /**
         * @brief Adds a new enum to this table
         */
        SqEnum addEnum(const char* name);
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
        * @brief Copy assingment operator
        */
        SqVM& operator = (const SqVM& other) = delete;
        /**
        * @brief Move assingment operator
        */
        SqVM& operator = (SqVM&& other) NOEXCEPT;
    private:
        std::unique_ptr<SqCompileException> compileException;
        std::unique_ptr<SqRuntimeException> runtimeException;

        static void pushArgs();

        template <class First, class... Rest> 
        void pushArgs(First&& first, Rest&&... rest) const {
            detail::push(vm, first);
            pushArgs(std::forward<Rest>(rest)...);
        }

        SqObject callAndReturn(SQUnsignedInteger nparams, SQInteger top) const;

        static void defaultPrintFunc(HSQUIRRELVM vm, const SQChar *s, ...);

        static void defaultErrorFunc(HSQUIRRELVM vm, const SQChar *s, ...);

        static SQInteger defaultRuntimeErrorFunc(HSQUIRRELVM vm);

        static void defaultCompilerErrorFunc(HSQUIRRELVM vm, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column);
    };

    /*namespace detail {
        template<class T>
        inline HSQOBJECT& detail::getClassObjectOrCreate(HSQUIRRELVM vm) {
            SqVM* machine = reinterpret_cast<SqVM*>(sq_getforeignptr(vm));
            return machine->getClassObjectOrCreate<T>();
        }
    }*/
}

#endif