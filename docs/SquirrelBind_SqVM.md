SqVM
===================================

Squirrel Virtual Machine object. 

**Inherits from:** [SquirrelBind::SqTable](SquirrelBind_SqTable.html)

The documentation for this class was generated from: `include/squirrelbind/vm.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqVM](#5acca1c3) (size_t _stackSize_, SqLibs::Flag _flags_ = 0x00)  _Creates a VM with a fixed stack size._ |
|  void | [destroy](#72c8179d) ()  _Destroys the VM and all of this objects._ |
|  virtual  | [~SqVM](#779d1e71) ()  _Destructor._ |
|  void | [swap](#8924af52) ([SqVM](SquirrelBind_SqVM.html) & _other_)  _Swaps the contents of this VM with another one._ |
|   | [SqVM](#bf532e55) (const [SqVM](SquirrelBind_SqVM.html) & _other_) = delete  _Disabled copy constructor._ |
|   | [SqVM](#a7c84378) ([SqVM](SquirrelBind_SqVM.html) && _other_)  _Move constructor._ |
|  void | [registerStdlib](#14d421ff) (SqLibs::Flag _flags_)  _Registers standard template libraries._ |
|  void | [setPrintFunc](#99ed7302) (SqPrintFunc _printFunc_, SqErrorFunc _errorFunc_)  _Registers print and error functions._ |
|  void | [setRuntimeErrorFunc](#f4992312) (SqRuntimeErrorFunc _runtimeErrorFunc_)  _Registers runtime error function._ |
|  void | [setCompileErrorFunc](#e03c6a2f) (SqCompileErrorFunc _compileErrorFunc_)  _Registers compilation error function._ |
|  SQInteger | [getTop](#72752bf9) () const  _Returns the index of the top slot of the stack._ |
|  const [SqCompileException](SquirrelBind_SqCompileException.html) & | [getLastCompileException](#d1ae9b00) () const  _Returns the last compilation exception._ |
|  const [SqRuntimeException](SquirrelBind_SqRuntimeException.html) & | [getLastRuntimeException](#aeb0d50b) () const  _Returns the last runtime exception._ |
|  [SqScript](SquirrelBind_SqScript.html) | [compileSource](#780a6141) (const char * _source_, const char * _name_ = "buffer")  _Compiles a script from a memory._ |
|  [SqScript](SquirrelBind_SqScript.html) | [compileFile](#24d3f612) (const char * _path_)  _Compiles a script from a source file._ |
|  void | [run](#1c0ab9fa) (const [SqScript](SquirrelBind_SqScript.html) & _script_) const  _Runs a script._ |
|  [SqObject](SquirrelBind_SqObject.html) | [callFunc](#12949825) (const [SqFunction](SquirrelBind_SqFunction.html) & _func_, const [SqObject](SquirrelBind_SqObject.html) & _env_, Args &&... _args_) const  _Calls a global function._ |
|  [SqInstance](SquirrelBind_SqInstance.html) | [newInstance](#22527e76) (const [SqClass](SquirrelBind_SqClass.html) & _cls_, Args &&... _args_) const  _Creates a new instance of class and call constructor with given arguments._ |
|  [SqInstance](SquirrelBind_SqInstance.html) | [newInstanceNoCtor](#57eaf561) (const [SqClass](SquirrelBind_SqClass.html) & _cls_) const  _Creates a new instance of class without calling a constructor._ |
|  [SqTable](SquirrelBind_SqTable.html) | [newTable](#d544d4a1) () const  _Creates a new empty table._ |
|  [SqArray](SquirrelBind_SqArray.html) | [newArray](#0c1af95e) () const  _Creates a new empty array._ |
|  [SqArray](SquirrelBind_SqArray.html) | [newArray](#dc14dfbe) (const std::vector< T > & _vector_) const  _Creates a new array._ |
|  [SqEnum](SquirrelBind_SqEnum.html) | [addEnum](#ee2c8e16) (const char * _name_)  _Adds a new enum to this table._ |
|  void | [setConst](#9b6fb87a) (const char * _name_, const T & _value_)  _Adds a new constant key-value pair to this table._ |
|  void | [debugStack](#31f8c48f) () const  _Prints stack objects._ |
|  void | [addClassObj](#27c0bea4) (size_t _hashCode_, const HSQOBJECT & _obj_)  _Add registered class object into the table of known classes._ |
|  const HSQOBJECT & | [getClassObj](#1b9c1ed4) (size_t _hashCode_)  _Get registered class object from hash code._ |
|  [SqVM](SquirrelBind_SqVM.html) & | [operator=](#6798d965) (const [SqVM](SquirrelBind_SqVM.html) & _other_) = delete  _Copy assingment operator._ |
|  [SqVM](SquirrelBind_SqVM.html) & | [operator=](#a4d23f25) ([SqVM](SquirrelBind_SqVM.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="5acca1c3" href="#5acca1c3">SqVM</a>

```cpp
 SqVM (
    size_t stackSize,
    SqLibs::Flag flags = 0x00
) 
```

Creates a VM with a fixed stack size. 

### _function_ <a id="72c8179d" href="#72c8179d">destroy</a>

```cpp
void destroy () 
```

Destroys the VM and all of this objects. 

### _function_ <a id="779d1e71" href="#779d1e71">~SqVM</a>

```cpp
virtual  ~SqVM () 
```

Destructor. 

### _function_ <a id="8924af52" href="#8924af52">swap</a>

```cpp
void swap (
    SqVM & other
) 
```

Swaps the contents of this VM with another one. 

### _function_ <a id="bf532e55" href="#bf532e55">SqVM</a>

```cpp
 SqVM (
    const SqVM & other
) = delete 
```

Disabled copy constructor. 

### _function_ <a id="a7c84378" href="#a7c84378">SqVM</a>

```cpp
 SqVM (
    SqVM && other
) 
```

Move constructor. 

### _function_ <a id="14d421ff" href="#14d421ff">registerStdlib</a>

```cpp
void registerStdlib (
    SqLibs::Flag flags
) 
```

Registers standard template libraries. 

### _function_ <a id="99ed7302" href="#99ed7302">setPrintFunc</a>

```cpp
void setPrintFunc (
    SqPrintFunc printFunc,
    SqErrorFunc errorFunc
) 
```

Registers print and error functions. 

### _function_ <a id="f4992312" href="#f4992312">setRuntimeErrorFunc</a>

```cpp
void setRuntimeErrorFunc (
    SqRuntimeErrorFunc runtimeErrorFunc
) 
```

Registers runtime error function. 

### _function_ <a id="e03c6a2f" href="#e03c6a2f">setCompileErrorFunc</a>

```cpp
void setCompileErrorFunc (
    SqCompileErrorFunc compileErrorFunc
) 
```

Registers compilation error function. 

### _function_ <a id="72752bf9" href="#72752bf9">getTop</a>

```cpp
SQInteger getTop () const 
```

Returns the index of the top slot of the stack. 

### _function_ <a id="d1ae9b00" href="#d1ae9b00">getLastCompileException</a>

```cpp
inline const SqCompileException & getLastCompileException () const 
```

Returns the last compilation exception. 

### _function_ <a id="aeb0d50b" href="#aeb0d50b">getLastRuntimeException</a>

```cpp
inline const SqRuntimeException & getLastRuntimeException () const 
```

Returns the last runtime exception. 

### _function_ <a id="780a6141" href="#780a6141">compileSource</a>

```cpp
SqScript compileSource (
    const char * source,
    const char * name = "buffer"
) 
```

Compiles a script from a memory. 

The script can be associated with a name as a second parameter. This name is used during runtime error information. **Exceptions:**
* _[SqCompileException](SquirrelBind_SqCompileException.html):_ 


### _function_ <a id="24d3f612" href="#24d3f612">compileFile</a>

```cpp
SqScript compileFile (
    const char * path
) 
```

Compiles a script from a source file. 

**Exceptions:**
* _[SqCompileException](SquirrelBind_SqCompileException.html):_ 


### _function_ <a id="1c0ab9fa" href="#1c0ab9fa">run</a>

```cpp
void run (
    const SqScript & script
) const 
```

Runs a script. 

When the script runs for the first time, the contens such as class definitions are assigned to the root table (global table). **Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ 


### _function_ <a id="12949825" href="#12949825">callFunc</a>

```cpp
inline SqObject callFunc (
    const SqFunction & func,
    const SqObject & env,
    Args &&... args
) const 
```

Calls a global function. 

**Params:**
* _func:_ The instance of a function 
* _args:_ Any number of arguments 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if an exception is thrown or number of arguments do not match 
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if casting from Squirrel objects to C++ objects failed 


### _function_ <a id="22527e76" href="#22527e76">newInstance</a>

```cpp
inline SqInstance newInstance (
    const SqClass & cls,
    Args &&... args
) const 
```

Creates a new instance of class and call constructor with given arguments. 

**Params:**
* _cls:_ The object of a class 
* _args:_ Any number of arguments 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ 


### _function_ <a id="57eaf561" href="#57eaf561">newInstanceNoCtor</a>

```cpp
inline SqInstance newInstanceNoCtor (
    const SqClass & cls
) const 
```

Creates a new instance of class without calling a constructor. 

**Params:**
* _cls:_ The object of a class 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ 


### _function_ <a id="d544d4a1" href="#d544d4a1">newTable</a>

```cpp
inline SqTable newTable () const 
```

Creates a new empty table. 

### _function_ <a id="0c1af95e" href="#0c1af95e">newArray</a>

```cpp
inline SqArray newArray () const 
```

Creates a new empty array. 

### _function_ <a id="dc14dfbe" href="#dc14dfbe">newArray</a>

```cpp
inline SqArray newArray (
    const std::vector< T > & vector
) const 
```

Creates a new array. 

### _function_ <a id="ee2c8e16" href="#ee2c8e16">addEnum</a>

```cpp
SqEnum addEnum (
    const char * name
) 
```

Adds a new enum to this table. 

### _function_ <a id="9b6fb87a" href="#9b6fb87a">setConst</a>

```cpp
inline void setConst (
    const char * name,
    const T & value
) 
```

Adds a new constant key-value pair to this table. 

### _function_ <a id="31f8c48f" href="#31f8c48f">debugStack</a>

```cpp
void debugStack () const 
```

Prints stack objects. 

### _function_ <a id="27c0bea4" href="#27c0bea4">addClassObj</a>

```cpp
void addClassObj (
    size_t hashCode,
    const HSQOBJECT & obj
) 
```

Add registered class object into the table of known classes. 

### _function_ <a id="1b9c1ed4" href="#1b9c1ed4">getClassObj</a>

```cpp
const HSQOBJECT & getClassObj (
    size_t hashCode
) 
```

Get registered class object from hash code. 

### _function_ <a id="6798d965" href="#6798d965">operator=</a>

```cpp
SqVM & operator= (
    const SqVM & other
) = delete 
```

Copy assingment operator. 

### _function_ <a id="a4d23f25" href="#a4d23f25">operator=</a>

```cpp
SqVM & operator= (
    SqVM && other
) 
```

Move assingment operator. 



