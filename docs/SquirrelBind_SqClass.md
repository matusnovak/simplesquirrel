SqClass
===================================

Squirrel class object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

The documentation for this class was generated from: `include/squirrelbind/class.hpp`



## Classes

| Name |
|:-----|
| struct [SquirrelBind::SqClass::Ctor](SquirrelBind_SqClass_Ctor.html) <span style="opacity:0.8;">Constructor helper class. </span> |
| struct [SquirrelBind::SqClass::Ctor< T(Args...)>](SquirrelBind_SqClass_Ctor__T_Args_____.html) |


## Protected Attributes

| Type | Name |
| -------: | :------- |
|  [SqObject](SquirrelBind_SqObject.html) | [tableSet](#0c2aeb68) |
|  [SqObject](SquirrelBind_SqObject.html) | [tableGet](#a52615db) |


## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqClass](#519ba7f8) (HSQUIRRELVM _vm_)  _Creates a new empty class._ |
|  explicit  | [SqClass](#bb40e70f) (const [SqObject](SquirrelBind_SqObject.html) & _object_)  _Converts SqObject to class object._ |
|   | [SqClass](#ecedccac) (const [SqClass](SquirrelBind_SqClass.html) & _other_)  _Copy constructor._ |
|   | [SqClass](#2297073d) ([SqClass](SquirrelBind_SqClass.html) && _other_)  _Move constructor._ |
|  void | [swap](#d5e537c5) ([SqClass](SquirrelBind_SqClass.html) & _other_)  _Swaps two classes._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [findFunc](#248a91d6) (const char * _name_) const  _Finds a function in this class._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#1ad6d871) (const char * _name_, const std::function< Return(Object *, Args...)> & _func_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#cf0d9ec7) (const char * _name_, Return(Object::*)(Args...) _memfunc_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#e4812636) (const char * _name_, Return(Object::*)(Args...) const _memfunc_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [addFunc](#e0b60e6b) (const char * _name_, const F & _lambda_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  void | [addVar](#cf31ca91) (const std::string & _name_, V T::* _ptr_, bool _isStatic_ = false)  |
|  void | [addConstVar](#47f13da3) (const std::string & _name_, V T::* _ptr_, bool _isStatic_ = false)  |
|  [SqClass](SquirrelBind_SqClass.html) & | [operator=](#6b8f5cf9) (const [SqClass](SquirrelBind_SqClass.html) & _other_)  _Copy assingment operator._ |
|  [SqClass](SquirrelBind_SqClass.html) & | [operator=](#350ac320) ([SqClass](SquirrelBind_SqClass.html) && _other_)  _Move assingment operator._ |


## Protected Functions

| Type | Name |
| -------: | :------- |
|  void | [findTable](#0c93d0e9) (const char * _name_, [SqObject](SquirrelBind_SqObject.html) & _table_, SQFUNCTION _dlg_) const  |
|  void | [bindVar](#7dfd29a4) (const std::string & _name_, V T::* _ptr_, HSQOBJECT & _table_, SQFUNCTION _stub_, bool _isStatic_)  |


## Protected Static Functions

| Type | Name |
| -------: | :------- |
|  SQInteger | [dlgGetStub](#4897ee58) (HSQUIRRELVM _vm_)  |
|  SQInteger | [dlgSetStub](#86360126) (HSQUIRRELVM _vm_)  |
|  SQInteger | [varGetStub](#7cb4c306) (HSQUIRRELVM _vm_)  |
|  SQInteger | [varSetStub](#87bac8be) (HSQUIRRELVM _vm_)  |


## Protected Attributes Documentation

### _variable_ <a id="0c2aeb68" href="#0c2aeb68">tableSet</a>

```cpp
SqObject tableSet
```



### _variable_ <a id="a52615db" href="#a52615db">tableGet</a>

```cpp
SqObject tableGet
```





## Public Functions Documentation

### _function_ <a id="519ba7f8" href="#519ba7f8">SqClass</a>

```cpp
 SqClass (
    HSQUIRRELVM vm
) 
```

Creates a new empty class. 

### _function_ <a id="bb40e70f" href="#bb40e70f">SqClass</a>

```cpp
explicit  SqClass (
    const SqObject & object
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to class object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of a class 


### _function_ <a id="ecedccac" href="#ecedccac">SqClass</a>

```cpp
 SqClass (
    const SqClass & other
) 
```

Copy constructor. 

### _function_ <a id="2297073d" href="#2297073d">SqClass</a>

```cpp
 SqClass (
    SqClass && other
) 
```

Move constructor. 

### _function_ <a id="d5e537c5" href="#d5e537c5">swap</a>

```cpp
void swap (
    SqClass & other
) 
```

Swaps two classes. 

### _function_ <a id="248a91d6" href="#248a91d6">findFunc</a>

```cpp
SqFunction findFunc (
    const char * name
) const 
```

Finds a function in this class. 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 
* _[SqNotFoundException](SquirrelBind_SqNotFoundException.html):_ if function was not found 
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the object found is not a function 


### _function_ <a id="1ad6d871" href="#1ad6d871">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    const std::function< Return(Object *, Args...)> & func,
    bool isStatic = false
) 
```

Adds a new function type to this class. 

**Params:**
* _name:_ Name of the function to add 
* _func:_ std::function that contains "this" pointer to the class type followed by any number of arguments with any type 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the added function 


### _function_ <a id="cf0d9ec7" href="#cf0d9ec7">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    Return(Object::*)(Args...) memfunc,
    bool isStatic = false
) 
```

Adds a new function type to this class. 

**Params:**
* _name:_ Name of the function to add 
* _memfunc:_ Pointer to member function 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the added function 


### _function_ <a id="e4812636" href="#e4812636">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    Return(Object::*)(Args...) const memfunc,
    bool isStatic = false
) 
```

Adds a new function type to this class. 

**Params:**
* _name:_ Name of the function to add 
* _memfunc:_ Pointer to constant member function 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the added function 


### _function_ <a id="e0b60e6b" href="#e0b60e6b">addFunc</a>

```cpp
inline SqFunction addFunc (
    const char * name,
    const F & lambda,
    bool isStatic = false
) 
```

Adds a new function type to this class. 

**Params:**
* _name:_ Name of the function to add 
* _lambda:_ Lambda function that contains "this" pointer to the class type followed by any number of arguments with any type 

**Exceptions:**
* _[SqRuntimeException](SquirrelBind_SqRuntimeException.html):_ if VM is invalid 

**return:** [SqFunction](SquirrelBind_SqFunction.html) object references the added function 


### _function_ <a id="cf31ca91" href="#cf31ca91">addVar</a>

```cpp
inline void addVar (
    const std::string & name,
    V T::* ptr,
    bool isStatic = false
) 
```



### _function_ <a id="47f13da3" href="#47f13da3">addConstVar</a>

```cpp
inline void addConstVar (
    const std::string & name,
    V T::* ptr,
    bool isStatic = false
) 
```



### _function_ <a id="6b8f5cf9" href="#6b8f5cf9">operator=</a>

```cpp
SqClass & operator= (
    const SqClass & other
) 
```

Copy assingment operator. 

### _function_ <a id="350ac320" href="#350ac320">operator=</a>

```cpp
SqClass & operator= (
    SqClass && other
) 
```

Move assingment operator. 



## Protected Functions Documentation

### _function_ <a id="0c93d0e9" href="#0c93d0e9">findTable</a>

```cpp
void findTable (
    const char * name,
    SqObject & table,
    SQFUNCTION dlg
) const 
```



### _function_ <a id="7dfd29a4" href="#7dfd29a4">bindVar</a>

```cpp
inline void bindVar (
    const std::string & name,
    V T::* ptr,
    HSQOBJECT & table,
    SQFUNCTION stub,
    bool isStatic
) 
```





## Protected Static Functions Documentation

### _function_ <a id="4897ee58" href="#4897ee58">dlgGetStub</a>

```cpp
static SQInteger dlgGetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="86360126" href="#86360126">dlgSetStub</a>

```cpp
static SQInteger dlgSetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="7cb4c306" href="#7cb4c306">varGetStub</a>

```cpp
static inline SQInteger varGetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="87bac8be" href="#87bac8be">varSetStub</a>

```cpp
static inline SQInteger varSetStub (
    HSQUIRRELVM vm
) 
```





