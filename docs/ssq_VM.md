VM
===================================

Squirrel Virtual Machine object. 

**Inherits from:** [ssq::Table](ssq_Table.html)

The documentation for this class was generated from: `include/simplesquirrel/vm.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [VM](#f45faf92) (size_t _stackSize_, Libs::Flag _flags_ = 0x00)  _Creates a VM with a fixed stack size._ |
|  void | [destroy](#c31c2839) ()  _Destroys the VM and all of this objects._ |
|  virtual  | [~VM](#295aa1b9) ()  _Destructor._ |
|  void | [swap](#d836eb0f) ([VM](ssq_VM.html) & _other_)  _Swaps the contents of this VM with another one._ |
|   | [VM](#f76bea4e) (const [VM](ssq_VM.html) & _other_) = delete  _Disabled copy constructor._ |
|   | [VM](#b97de12f) ([VM](ssq_VM.html) && _other_)  _Move constructor._ |
|  void | [registerStdlib](#eac28975) (Libs::Flag _flags_)  _Registers standard template libraries._ |
|  void | [setPrintFunc](#5c4156db) (SqPrintFunc _printFunc_, SqErrorFunc _errorFunc_)  _Registers print and error functions._ |
|  void | [setRuntimeErrorFunc](#3bf5bcae) (SqRuntimeErrorFunc _runtimeErrorFunc_)  _Registers runtime error function._ |
|  void | [setCompileErrorFunc](#a915dc09) (SqCompileErrorFunc _compileErrorFunc_)  _Registers compilation error function._ |
|  SQInteger | [getTop](#564238ed) () const  _Returns the index of the top slot of the stack._ |
|  const [CompileException](ssq_CompileException.html) & | [getLastCompileException](#6066c8ab) () const  _Returns the last compilation exception._ |
|  const [RuntimeException](ssq_RuntimeException.html) & | [getLastRuntimeException](#72c1686d) () const  _Returns the last runtime exception._ |
|  [Script](ssq_Script.html) | [compileSource](#98c74cef) (const char * _source_, const char * _name_ = "buffer")  _Compiles a script from a memory._ |
|  [Script](ssq_Script.html) | [compileFile](#9e1d5185) (const char * _path_)  _Compiles a script from a source file._ |
|  void | [run](#a867fa78) (const [Script](ssq_Script.html) & _script_) const  _Runs a script._ |
|  [Object](ssq_Object.html) | [callFunc](#d6669deb) (const [Function](ssq_Function.html) & _func_, const [Object](ssq_Object.html) & _env_, Args &&... _args_) const  _Calls a global function._ |
|  [Instance](ssq_Instance.html) | [newInstance](#75474b3c) (const [Class](ssq_Class.html) & _cls_, Args &&... _args_) const  _Creates a new instance of class and call constructor with given arguments._ |
|  [Instance](ssq_Instance.html) | [newInstanceNoCtor](#9d783eeb) (const [Class](ssq_Class.html) & _cls_) const  _Creates a new instance of class without calling a constructor._ |
|  [Table](ssq_Table.html) | [newTable](#e10b95d4) () const  _Creates a new empty table._ |
|  [Array](ssq_Array.html) | [newArray](#e4b3a226) () const  _Creates a new empty array._ |
|  [Array](ssq_Array.html) | [newArray](#22ba6a6f) (const std::vector< T > & _vector_) const  _Creates a new array._ |
|  [Enum](ssq_Enum.html) | [addEnum](#f332838a) (const char * _name_)  _Adds a new enum to this table._ |
|  void | [setConst](#8f652c13) (const char * _name_, const T & _value_)  _Adds a new constant key-value pair to this table._ |
|  void | [debugStack](#94f3c01e) () const  _Prints stack objects._ |
|  void | [addClassObj](#50723b6b) (size_t _hashCode_, const HSQOBJECT & _obj_)  _Add registered class object into the table of known classes._ |
|  const HSQOBJECT & | [getClassObj](#a177d2a9) (size_t _hashCode_)  _Get registered class object from hash code._ |
|  [VM](ssq_VM.html) & | [operator=](#8401b6bc) (const [VM](ssq_VM.html) & _other_) = delete  _Copy assingment operator._ |
|  [VM](ssq_VM.html) & | [operator=](#41ae59e4) ([VM](ssq_VM.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="f45faf92" href="#f45faf92">VM</a>

```cpp
 VM (
    size_t stackSize,
    Libs::Flag flags = 0x00
) 
```

Creates a [VM](ssq_VM.html) with a fixed stack size. 

### _function_ <a id="c31c2839" href="#c31c2839">destroy</a>

```cpp
void destroy () 
```

Destroys the [VM](ssq_VM.html) and all of this objects. 

### _function_ <a id="295aa1b9" href="#295aa1b9">~VM</a>

```cpp
virtual  ~VM () 
```

Destructor. 

### _function_ <a id="d836eb0f" href="#d836eb0f">swap</a>

```cpp
void swap (
    VM & other
) 
```

Swaps the contents of this [VM](ssq_VM.html) with another one. 

### _function_ <a id="f76bea4e" href="#f76bea4e">VM</a>

```cpp
 VM (
    const VM & other
) = delete 
```

Disabled copy constructor. 

### _function_ <a id="b97de12f" href="#b97de12f">VM</a>

```cpp
 VM (
    VM && other
) 
```

Move constructor. 

### _function_ <a id="eac28975" href="#eac28975">registerStdlib</a>

```cpp
void registerStdlib (
    Libs::Flag flags
) 
```

Registers standard template libraries. 

### _function_ <a id="5c4156db" href="#5c4156db">setPrintFunc</a>

```cpp
void setPrintFunc (
    SqPrintFunc printFunc,
    SqErrorFunc errorFunc
) 
```

Registers print and error functions. 

### _function_ <a id="3bf5bcae" href="#3bf5bcae">setRuntimeErrorFunc</a>

```cpp
void setRuntimeErrorFunc (
    SqRuntimeErrorFunc runtimeErrorFunc
) 
```

Registers runtime error function. 

### _function_ <a id="a915dc09" href="#a915dc09">setCompileErrorFunc</a>

```cpp
void setCompileErrorFunc (
    SqCompileErrorFunc compileErrorFunc
) 
```

Registers compilation error function. 

### _function_ <a id="564238ed" href="#564238ed">getTop</a>

```cpp
SQInteger getTop () const 
```

Returns the index of the top slot of the stack. 

### _function_ <a id="6066c8ab" href="#6066c8ab">getLastCompileException</a>

```cpp
inline const CompileException & getLastCompileException () const 
```

Returns the last compilation exception. 

### _function_ <a id="72c1686d" href="#72c1686d">getLastRuntimeException</a>

```cpp
inline const RuntimeException & getLastRuntimeException () const 
```

Returns the last runtime exception. 

### _function_ <a id="98c74cef" href="#98c74cef">compileSource</a>

```cpp
Script compileSource (
    const char * source,
    const char * name = "buffer"
) 
```

Compiles a script from a memory. 

The script can be associated with a name as a second parameter. This name is used during runtime error information. **Exceptions:**
* _[CompileException](ssq_CompileException.html):_ 


### _function_ <a id="9e1d5185" href="#9e1d5185">compileFile</a>

```cpp
Script compileFile (
    const char * path
) 
```

Compiles a script from a source file. 

**Exceptions:**
* _[CompileException](ssq_CompileException.html):_ 


### _function_ <a id="a867fa78" href="#a867fa78">run</a>

```cpp
void run (
    const Script & script
) const 
```

Runs a script. 

When the script runs for the first time, the contens such as class definitions are assigned to the root table (global table). **Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ 


### _function_ <a id="d6669deb" href="#d6669deb">callFunc</a>

```cpp
inline Object callFunc (
    const Function & func,
    const Object & env,
    Args &&... args
) const 
```

Calls a global function. 

**Params:**
* _func:_ The instance of a function 
* _args:_ Any number of arguments 

**Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ if an exception is thrown or number of arguments do not match 
* _[TypeException](ssq_TypeException.html):_ if casting from Squirrel objects to C++ objects failed 


### _function_ <a id="75474b3c" href="#75474b3c">newInstance</a>

```cpp
inline Instance newInstance (
    const Class & cls,
    Args &&... args
) const 
```

Creates a new instance of class and call constructor with given arguments. 

**Params:**
* _cls:_ The object of a class 
* _args:_ Any number of arguments 

**Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ 


### _function_ <a id="9d783eeb" href="#9d783eeb">newInstanceNoCtor</a>

```cpp
inline Instance newInstanceNoCtor (
    const Class & cls
) const 
```

Creates a new instance of class without calling a constructor. 

**Params:**
* _cls:_ The object of a class 

**Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ 


### _function_ <a id="e10b95d4" href="#e10b95d4">newTable</a>

```cpp
inline Table newTable () const 
```

Creates a new empty table. 

### _function_ <a id="e4b3a226" href="#e4b3a226">newArray</a>

```cpp
inline Array newArray () const 
```

Creates a new empty array. 

### _function_ <a id="22ba6a6f" href="#22ba6a6f">newArray</a>

```cpp
inline Array newArray (
    const std::vector< T > & vector
) const 
```

Creates a new array. 

### _function_ <a id="f332838a" href="#f332838a">addEnum</a>

```cpp
Enum addEnum (
    const char * name
) 
```

Adds a new enum to this table. 

### _function_ <a id="8f652c13" href="#8f652c13">setConst</a>

```cpp
inline void setConst (
    const char * name,
    const T & value
) 
```

Adds a new constant key-value pair to this table. 

### _function_ <a id="94f3c01e" href="#94f3c01e">debugStack</a>

```cpp
void debugStack () const 
```

Prints stack objects. 

### _function_ <a id="50723b6b" href="#50723b6b">addClassObj</a>

```cpp
void addClassObj (
    size_t hashCode,
    const HSQOBJECT & obj
) 
```

Add registered class object into the table of known classes. 

### _function_ <a id="a177d2a9" href="#a177d2a9">getClassObj</a>

```cpp
const HSQOBJECT & getClassObj (
    size_t hashCode
) 
```

Get registered class object from hash code. 

### _function_ <a id="8401b6bc" href="#8401b6bc">operator=</a>

```cpp
VM & operator= (
    const VM & other
) = delete 
```

Copy assingment operator. 

### _function_ <a id="41ae59e4" href="#41ae59e4">operator=</a>

```cpp
VM & operator= (
    VM && other
) 
```

Move assingment operator. 



