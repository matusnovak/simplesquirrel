SqEnum
===================================

Squirrel table object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

The documentation for this class was generated from: `include/squirrelbind/enum.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqEnum](#0556f70a) ()  _Creates empty table with null VM._ |
|  explicit  | [SqEnum](#d1d546c0) (const [SqObject](SquirrelBind_SqObject.html) & _other_)  _Converts SqObject to SqEnum._ |
|  explicit  | [SqEnum](#f3f1589b) (HSQUIRRELVM _vm_)  _Creates empty enum._ |
|   | [SqEnum](#625568c6) (const [SqEnum](SquirrelBind_SqEnum.html) & _other_)  _Copy constructor._ |
|   | [SqEnum](#ca04cb4c) ([SqEnum](SquirrelBind_SqEnum.html) && _other_)  _Move constructor._ |
|  void | [addSlot](#945e5c12) (const char * _name_, const T & _value_)  _Adds a new key-value pair to this table._ |
|  [SqEnum](SquirrelBind_SqEnum.html) & | [operator=](#36f4cdcc) (const [SqEnum](SquirrelBind_SqEnum.html) & _other_)  _Copy assingment operator._ |
|  [SqEnum](SquirrelBind_SqEnum.html) & | [operator=](#7ba2c2f1) ([SqEnum](SquirrelBind_SqEnum.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="0556f70a" href="#0556f70a">SqEnum</a>

```cpp
 SqEnum () 
```

Creates empty table with null VM. 

**note:** This object will be unusable 


### _function_ <a id="d1d546c0" href="#d1d546c0">SqEnum</a>

```cpp
explicit  SqEnum (
    const SqObject & other
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to [SqEnum](SquirrelBind_SqEnum.html). 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of an enum (table) 


### _function_ <a id="f3f1589b" href="#f3f1589b">SqEnum</a>

```cpp
explicit  SqEnum (
    HSQUIRRELVM vm
) 
```

Creates empty enum. 

### _function_ <a id="625568c6" href="#625568c6">SqEnum</a>

```cpp
 SqEnum (
    const SqEnum & other
) 
```

Copy constructor. 

### _function_ <a id="ca04cb4c" href="#ca04cb4c">SqEnum</a>

```cpp
 SqEnum (
    SqEnum && other
) 
```

Move constructor. 

### _function_ <a id="945e5c12" href="#945e5c12">addSlot</a>

```cpp
inline void addSlot (
    const char * name,
    const T & value
) 
```

Adds a new key-value pair to this table. 

### _function_ <a id="36f4cdcc" href="#36f4cdcc">operator=</a>

```cpp
SqEnum & operator= (
    const SqEnum & other
) 
```

Copy assingment operator. 

### _function_ <a id="7ba2c2f1" href="#7ba2c2f1">operator=</a>

```cpp
SqEnum & operator= (
    SqEnum && other
) 
```

Move assingment operator. 



