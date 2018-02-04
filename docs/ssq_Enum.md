Enum
===================================

Squirrel table object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

The documentation for this class was generated from: `include/simplesquirrel/enum.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Enum](#597a7df4) ()  _Creates empty table with null VM._ |
|  virtual  | [~Enum](#0079bfe3) ()  _Destructor._ |
|  explicit  | [Enum](#57a8d31d) (const [Object](ssq_Object.html) & _other_)  _Converts Object to Enum._ |
|  explicit  | [Enum](#a48ab63f) (HSQUIRRELVM _vm_)  _Creates empty enum._ |
|   | [Enum](#2982d363) (const [Enum](ssq_Enum.html) & _other_)  _Copy constructor._ |
|   | [Enum](#03cd8aff) ([Enum](ssq_Enum.html) && _other_)  _Move constructor._ |
|  void | [addSlot](#334fafae) (const char * _name_, const T & _value_)  _Adds a new key-value pair to this table._ |
|  [Enum](ssq_Enum.html) & | [operator=](#47b936cf) (const [Enum](ssq_Enum.html) & _other_)  _Copy assingment operator._ |
|  [Enum](ssq_Enum.html) & | [operator=](#c04c68a5) ([Enum](ssq_Enum.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="597a7df4" href="#597a7df4">Enum</a>

```cpp
 Enum () 
```

Creates empty table with null [VM](ssq_VM.html). 

**note:** This object will be unusable 


### _function_ <a id="0079bfe3" href="#0079bfe3">~Enum</a>

```cpp
virtual  ~Enum () 
```

Destructor. 

### _function_ <a id="57a8d31d" href="#57a8d31d">Enum</a>

```cpp
explicit  Enum (
    const Object & other
) 
```

Converts [Object](ssq_Object.html) to [Enum](ssq_Enum.html). 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of an enum (table) 


### _function_ <a id="a48ab63f" href="#a48ab63f">Enum</a>

```cpp
explicit  Enum (
    HSQUIRRELVM vm
) 
```

Creates empty enum. 

### _function_ <a id="2982d363" href="#2982d363">Enum</a>

```cpp
 Enum (
    const Enum & other
) 
```

Copy constructor. 

### _function_ <a id="03cd8aff" href="#03cd8aff">Enum</a>

```cpp
 Enum (
    Enum && other
) 
```

Move constructor. 

### _function_ <a id="334fafae" href="#334fafae">addSlot</a>

```cpp
inline void addSlot (
    const char * name,
    const T & value
) 
```

Adds a new key-value pair to this table. 

### _function_ <a id="47b936cf" href="#47b936cf">operator=</a>

```cpp
Enum & operator= (
    const Enum & other
) 
```

Copy assingment operator. 

### _function_ <a id="c04c68a5" href="#c04c68a5">operator=</a>

```cpp
Enum & operator= (
    Enum && other
) 
```

Move assingment operator. 



