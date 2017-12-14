SqTable
===================================

Squirrel table object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

**Implemented by:** [SquirrelBind::SqVM](SquirrelBind_SqVM.html)

The documentation for this class was generated from: `include/squirrelbind/table.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqTable](#206a6578) ()  _Creates empty table with null VM._ |
|  explicit  | [SqTable](#33a84383) (const [SqObject](SquirrelBind_SqObject.html) & _other_)  _Converts SqObject to SqTable._ |
|  explicit  | [SqTable](#3d340655) (HSQUIRRELVM _vm_)  _Creates empty table._ |
|   | [SqTable](#dad073b9) (const [SqTable](SquirrelBind_SqTable.html) & _other_)  _Copy constructor._ |
|   | [SqTable](#8ed096d5) ([SqTable](SquirrelBind_SqTable.html) && _other_)  _Move constructor._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [findFunc](#60fc935b) (const char * _name_) const  _Finds a function in this table._ |
|  [SqClass](SquirrelBind_SqClass.html) | [findClass](#bdf75573) (const char * _name_) const  _Finds a class in this table._ |
|  [SqClass](SquirrelBind_SqClass.html) | [addClass](#e60779b4) (const char * _name_, const std::function< T *(Args...)> & _allocator_ = std::bind(&detail::defaultClassAllocator< T >), bool _release_ = true)  _Adds a new class type to this table._ |
|  [SqClass](SquirrelBind_SqClass.html) | [addClass](#7f0af2a9) (const char * _name_, const [SqClass::Ctor](SquirrelBind_SqClass_Ctor.html)< T(Args...)> & _constructor_, bool _release_ = true)  _Adds a new class type to this table._ |
|  [SqClass](SquirrelBind_SqClass.html) | [addClass](#610e55fd) (const char * _name_, const F & _lambda_, bool _release_ = true)  _Adds a new class type to this table._ |
|  [SqClass](SquirrelBind_SqClass.html) | [addAbstractClass](#c213d866) (const char * _name_)  _Adds a new abstract class type to this table._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#188ce7b1) (const char * _name_, const std::function< R(Args...)> & _func_)  _Adds a new function type to this table._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#0bb526b4) (const char * _name_, const F & _lambda_)  _Adds a new lambda type to this table._ |
|  void | [set](#e28aa61a) (const char * _name_, const T & _value_)  _Adds a new key-value pair to this table._ |
|  T | [get](#d54b09fd) (const char * _name_)  |
|  size_t | [size](#dc6821f1) ()  |
|  [SqTable](SquirrelBind_SqTable.html) | [addTable](#a279e7e9) (const char * _name_)  _Adds a new table to this table._ |
|  [SqTable](SquirrelBind_SqTable.html) & | [operator=](#2a286544) (const [SqTable](SquirrelBind_SqTable.html) & _other_)  _Copy assingment operator._ |
|  [SqTable](SquirrelBind_SqTable.html) & | [operator=](#5b20f7fc) ([SqTable](SquirrelBind_SqTable.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="206a6578" href="#206a6578">SqTable</a>

```cpp
 SqTable () 
```

Creates empty table with null VM. 

**note:** This object will be unusable 


### _function_ <a id="33a84383" href="#33a84383">SqTable</a>

```cpp
explicit  SqTable (
    const SqObject & other
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to [SqTable](SquirrelBind_SqTable.html). 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of a table 


### _function_ <a id="3d340655" href="#3d340655">SqTable</a>

```cpp
explicit  SqTable (
    HSQUIRRELVM vm
) 
```

Creates empty table. 

### _function_ <a id="dad073b9" href="#dad073b9">SqTable</a>

```cpp
 SqTable (
    const SqTable & other
) 
```

Copy constructor. 

### _function_ <a id="8ed096d5" href="#8ed096d5">SqTable</a>

```cpp
 SqTable (
    SqTable && other
) 
```

Move constructor. 

### _function_ <a id="60fc935b" href="#60fc935b">findFunc</a>

```cpp
SqFunction findFunc (
    const char * name
) const 
```

Finds a function in this table. 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 
* _[SqNotFoundException](SquirrelBind_SqNotFoundException.html):_ if function was not found 
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the object found is not a function 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the found function 


### _function_ <a id="bdf75573" href="#bdf75573">findClass</a>

```cpp
SqClass findClass (
    const char * name
) const 
```

Finds a class in this table. 

**Exceptions:**
* _[SqNotFoundException](SquirrelBind_SqNotFoundException.html):_ if function was not found 
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the object found is not a function 

**return:** [SqClass](SquirrelBind_SqClass.html) object references the found class 


### _function_ <a id="e60779b4" href="#e60779b4">addClass</a>

```cpp
inline SqClass addClass (
    const char * name,
    const std::function< T *(Args...)> & allocator = std::bind(&detail::defaultClassAllocator< T >),
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [SqClass](SquirrelBind_SqClass.html) object references the added class 


### _function_ <a id="7f0af2a9" href="#7f0af2a9">addClass</a>

```cpp
inline SqClass addClass (
    const char * name,
    const SqClass::Ctor< T(Args...)> & constructor,
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [SqClass](SquirrelBind_SqClass.html) object references the added class 


### _function_ <a id="610e55fd" href="#610e55fd">addClass</a>

```cpp
inline SqClass addClass (
    const char * name,
    const F & lambda,
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [SqClass](SquirrelBind_SqClass.html) object references the added class 


### _function_ <a id="c213d866" href="#c213d866">addAbstractClass</a>

```cpp
inline SqClass addAbstractClass (
    const char * name
) 
```

Adds a new abstract class type to this table. 

**return:** [SqClass](SquirrelBind_SqClass.html) object references the added class 


### _function_ <a id="188ce7b1" href="#188ce7b1">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    const std::function< R(Args...)> & func
) 
```

Adds a new function type to this table. 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the added function 


### _function_ <a id="0bb526b4" href="#0bb526b4">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    const F & lambda
) 
```

Adds a new lambda type to this table. 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object that references the added function 


### _function_ <a id="e28aa61a" href="#e28aa61a">set</a>

```cpp
inline void set (
    const char * name,
    const T & value
) 
```

Adds a new key-value pair to this table. 

### _function_ <a id="d54b09fd" href="#d54b09fd">get</a>

```cpp
inline T get (
    const char * name
) 
```



### _function_ <a id="dc6821f1" href="#dc6821f1">size</a>

```cpp
size_t size () 
```



### _function_ <a id="a279e7e9" href="#a279e7e9">addTable</a>

```cpp
SqTable addTable (
    const char * name
) 
```

Adds a new table to this table. 

### _function_ <a id="2a286544" href="#2a286544">operator=</a>

```cpp
SqTable & operator= (
    const SqTable & other
) 
```

Copy assingment operator. 

### _function_ <a id="5b20f7fc" href="#5b20f7fc">operator=</a>

```cpp
SqTable & operator= (
    SqTable && other
) 
```

Move assingment operator. 



