Table
===================================

Squirrel table object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

**Implemented by:** [ssq::VM](ssq_VM.html)

The documentation for this class was generated from: `include/simplesquirrel/table.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Table](#35793f96) ()  _Creates empty table with null VM._ |
|  virtual  | [~Table](#358260c2) ()  _Destructor._ |
|  explicit  | [Table](#b2ad3f9b) (const [Object](ssq_Object.html) & _other_)  _Converts Object to Table._ |
|  explicit  | [Table](#e19999cf) (HSQUIRRELVM _vm_)  _Creates empty table._ |
|   | [Table](#9128b406) (const [Table](ssq_Table.html) & _other_)  _Copy constructor._ |
|   | [Table](#7150e815) ([Table](ssq_Table.html) && _other_)  _Move constructor._ |
|  [Function](ssq_Function.html) | [findFunc](#504c1d56) (const char * _name_) const  _Finds a function in this table._ |
|  [Class](ssq_Class.html) | [findClass](#08769647) (const char * _name_) const  _Finds a class in this table._ |
|  [Class](ssq_Class.html) | [addClass](#239cdac2) (const char * _name_, const std::function< T *(Args...)> & _allocator_ = std::bind(&detail::defaultClassAllocator< T >), bool _release_ = true)  _Adds a new class type to this table._ |
|  [Class](ssq_Class.html) | [addClass](#5cf06d6f) (const char * _name_, const [Class::Ctor](ssq_Class_Ctor.html)< T(Args...)> & _constructor_, bool _release_ = true)  _Adds a new class type to this table._ |
|  [Class](ssq_Class.html) | [addClass](#5a50228c) (const char * _name_, const F & _lambda_, bool _release_ = true)  _Adds a new class type to this table._ |
|  [Class](ssq_Class.html) | [addAbstractClass](#56711c07) (const char * _name_)  _Adds a new abstract class type to this table._ |
|  [Function](ssq_Function.html) | [addFunc](#1ca86702) (const char * _name_, const std::function< R(Args...)> & _func_)  _Adds a new function type to this table._ |
|  [Function](ssq_Function.html) | [addFunc](#ca83ab71) (const char * _name_, const F & _lambda_)  _Adds a new lambda type to this table._ |
|  void | [set](#fa8c73bf) (const char * _name_, const T & _value_)  _Adds a new key-value pair to this table._ |
|  T | [get](#527c14e6) (const char * _name_)  |
|  size_t | [size](#1e46fb3e) ()  |
|  [Table](ssq_Table.html) | [addTable](#b68bbf11) (const char * _name_)  _Adds a new table to this table._ |
|  [Table](ssq_Table.html) & | [operator=](#730408b8) (const [Table](ssq_Table.html) & _other_)  _Copy assingment operator._ |
|  [Table](ssq_Table.html) & | [operator=](#3632523e) ([Table](ssq_Table.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="35793f96" href="#35793f96">Table</a>

```cpp
 Table () 
```

Creates empty table with null [VM](ssq_VM.html). 

**note:** This object will be unusable 


### _function_ <a id="358260c2" href="#358260c2">~Table</a>

```cpp
virtual  ~Table () 
```

Destructor. 

### _function_ <a id="b2ad3f9b" href="#b2ad3f9b">Table</a>

```cpp
explicit  Table (
    const Object & other
) 
```

Converts [Object](ssq_Object.html) to [Table](ssq_Table.html). 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of a table 


### _function_ <a id="e19999cf" href="#e19999cf">Table</a>

```cpp
explicit  Table (
    HSQUIRRELVM vm
) 
```

Creates empty table. 

### _function_ <a id="9128b406" href="#9128b406">Table</a>

```cpp
 Table (
    const Table & other
) 
```

Copy constructor. 

### _function_ <a id="7150e815" href="#7150e815">Table</a>

```cpp
 Table (
    Table && other
) 
```

Move constructor. 

### _function_ <a id="504c1d56" href="#504c1d56">findFunc</a>

```cpp
Function findFunc (
    const char * name
) const 
```

Finds a function in this table. 

**Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 
* _[NotFoundException](ssq_NotFoundException.html):_ if function was not found 
* _[TypeException](ssq_TypeException.html):_ if the object found is not a function 

**return:** [Function](ssq_Function.html) object references the found function 


### _function_ <a id="08769647" href="#08769647">findClass</a>

```cpp
Class findClass (
    const char * name
) const 
```

Finds a class in this table. 

**Exceptions:**
* _[NotFoundException](ssq_NotFoundException.html):_ if function was not found 
* _[TypeException](ssq_TypeException.html):_ if the object found is not a function 

**return:** [Class](ssq_Class.html) object references the found class 


### _function_ <a id="239cdac2" href="#239cdac2">addClass</a>

```cpp
inline Class addClass (
    const char * name,
    const std::function< T *(Args...)> & allocator = std::bind(&detail::defaultClassAllocator< T >),
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [Class](ssq_Class.html) object references the added class 


### _function_ <a id="5cf06d6f" href="#5cf06d6f">addClass</a>

```cpp
inline Class addClass (
    const char * name,
    const Class::Ctor< T(Args...)> & constructor,
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [Class](ssq_Class.html) object references the added class 


### _function_ <a id="5a50228c" href="#5a50228c">addClass</a>

```cpp
inline Class addClass (
    const char * name,
    const F & lambda,
    bool release = true
) 
```

Adds a new class type to this table. 

**return:** [Class](ssq_Class.html) object references the added class 


### _function_ <a id="56711c07" href="#56711c07">addAbstractClass</a>

```cpp
inline Class addAbstractClass (
    const char * name
) 
```

Adds a new abstract class type to this table. 

**return:** [Class](ssq_Class.html) object references the added class 


### _function_ <a id="1ca86702" href="#1ca86702">addFunc</a>

```cpp
inline Function addFunc (
    const char * name,
    const std::function< R(Args...)> & func
) 
```

Adds a new function type to this table. 

**return:** [Function](ssq_Function.html) object references the added function 


### _function_ <a id="ca83ab71" href="#ca83ab71">addFunc</a>

```cpp
inline Function addFunc (
    const char * name,
    const F & lambda
) 
```

Adds a new lambda type to this table. 

**return:** [Function](ssq_Function.html) object that references the added function 


### _function_ <a id="fa8c73bf" href="#fa8c73bf">set</a>

```cpp
inline void set (
    const char * name,
    const T & value
) 
```

Adds a new key-value pair to this table. 

### _function_ <a id="527c14e6" href="#527c14e6">get</a>

```cpp
inline T get (
    const char * name
) 
```



### _function_ <a id="1e46fb3e" href="#1e46fb3e">size</a>

```cpp
size_t size () 
```



### _function_ <a id="b68bbf11" href="#b68bbf11">addTable</a>

```cpp
Table addTable (
    const char * name
) 
```

Adds a new table to this table. 

### _function_ <a id="730408b8" href="#730408b8">operator=</a>

```cpp
Table & operator= (
    const Table & other
) 
```

Copy assingment operator. 

### _function_ <a id="3632523e" href="#3632523e">operator=</a>

```cpp
Table & operator= (
    Table && other
) 
```

Move assingment operator. 



