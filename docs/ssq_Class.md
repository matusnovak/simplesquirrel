Class
===================================

Squirrel class object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

The documentation for this class was generated from: `include/simplesquirrel/class.hpp`



## Classes

| Name |
|:-----|
| struct [ssq::Class::Ctor](ssq_Class_Ctor.html) <span style="opacity:0.8;">Constructor helper class. </span> |
| struct [ssq::Class::Ctor< T(Args...)>](ssq_Class_Ctor__T_Args_____.html) |


## Protected Attributes

| Type | Name |
| -------: | :------- |
|  [Object](ssq_Object.html) | [tableSet](#8a12bdb2) |
|  [Object](ssq_Object.html) | [tableGet](#71df95f9) |


## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Class](#2127620e) ()  _Creates an empty invalid class._ |
|  virtual  | [~Class](#c0664c1c) ()  _Destructor._ |
|   | [Class](#1ff9afc5) (HSQUIRRELVM _vm_)  _Creates a new empty class._ |
|  explicit  | [Class](#becd4d0b) (const [Object](ssq_Object.html) & _object_)  _Converts Object to class object._ |
|   | [Class](#f57a1e29) (const [Class](ssq_Class.html) & _other_)  _Copy constructor._ |
|   | [Class](#0e964d26) ([Class](ssq_Class.html) && _other_)  _Move constructor._ |
|  void | [swap](#55bb1a9a) ([Class](ssq_Class.html) & _other_)  _Swaps two classes._ |
|  [Function](ssq_Function.html) | [findFunc](#dfb03a66) (const char * _name_) const  _Finds a function in this class._ |
|  [Function](ssq_Function.html) | [addFunc](#25315c26) (const char * _name_, const std::function< Return([Object](ssq_Object.html) *, Args...)> & _func_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [Function](ssq_Function.html) | [addFunc](#108921a4) (const char * _name_, Return(Object::*)(Args...) _memfunc_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [Function](ssq_Function.html) | [addFunc](#4c484368) (const char * _name_, Return(Object::*)(Args...) const _memfunc_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  [Function](ssq_Function.html) | [addFunc](#bb85e908) (const char * _name_, const F & _lambda_, bool _isStatic_ = false)  _Adds a new function type to this class._ |
|  void | [addVar](#5038ab76) (const std::string & _name_, V T::* _ptr_, bool _isStatic_ = false)  |
|  void | [addConstVar](#f977f9bf) (const std::string & _name_, V T::* _ptr_, bool _isStatic_ = false)  |
|  [Class](ssq_Class.html) & | [operator=](#4b36c7c3) (const [Class](ssq_Class.html) & _other_)  _Copy assingment operator._ |
|  [Class](ssq_Class.html) & | [operator=](#bf272d20) ([Class](ssq_Class.html) && _other_)  _Move assingment operator._ |


## Protected Functions

| Type | Name |
| -------: | :------- |
|  void | [findTable](#21a84198) (const char * _name_, [Object](ssq_Object.html) & _table_, SQFUNCTION _dlg_) const  |
|  void | [bindVar](#6ee153ab) (const std::string & _name_, V T::* _ptr_, HSQOBJECT & _table_, SQFUNCTION _stub_, bool _isStatic_)  |


## Protected Static Functions

| Type | Name |
| -------: | :------- |
|  SQInteger | [dlgGetStub](#223cda5a) (HSQUIRRELVM _vm_)  |
|  SQInteger | [dlgSetStub](#4a11e24a) (HSQUIRRELVM _vm_)  |
|  SQInteger | [varGetStub](#35272128) (HSQUIRRELVM _vm_)  |
|  SQInteger | [varSetStub](#1c7c3db5) (HSQUIRRELVM _vm_)  |


## Protected Attributes Documentation

### _variable_ <a id="8a12bdb2" href="#8a12bdb2">tableSet</a>

```cpp
Object tableSet
```



### _variable_ <a id="71df95f9" href="#71df95f9">tableGet</a>

```cpp
Object tableGet
```





## Public Functions Documentation

### _function_ <a id="2127620e" href="#2127620e">Class</a>

```cpp
 Class () 
```

Creates an empty invalid class. 

### _function_ <a id="c0664c1c" href="#c0664c1c">~Class</a>

```cpp
virtual  ~Class () 
```

Destructor. 

### _function_ <a id="1ff9afc5" href="#1ff9afc5">Class</a>

```cpp
 Class (
    HSQUIRRELVM vm
) 
```

Creates a new empty class. 

### _function_ <a id="becd4d0b" href="#becd4d0b">Class</a>

```cpp
explicit  Class (
    const Object & object
) 
```

Converts [Object](ssq_Object.html) to class object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of a class 


### _function_ <a id="f57a1e29" href="#f57a1e29">Class</a>

```cpp
 Class (
    const Class & other
) 
```

Copy constructor. 

### _function_ <a id="0e964d26" href="#0e964d26">Class</a>

```cpp
 Class (
    Class && other
) 
```

Move constructor. 

### _function_ <a id="55bb1a9a" href="#55bb1a9a">swap</a>

```cpp
void swap (
    Class & other
) 
```

Swaps two classes. 

### _function_ <a id="dfb03a66" href="#dfb03a66">findFunc</a>

```cpp
Function findFunc (
    const char * name
) const 
```

Finds a function in this class. 

**Exceptions:**
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 
* _[NotFoundException](ssq_NotFoundException.html):_ if function was not found 
* _[TypeException](ssq_TypeException.html):_ if the object found is not a function 


### _function_ <a id="25315c26" href="#25315c26">addFunc</a>

```cpp
inline Function addFunc (
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
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 

**return:** [Function](ssq_Function.html) object references the added function 


### _function_ <a id="108921a4" href="#108921a4">addFunc</a>

```cpp
inline Function addFunc (
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
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 

**return:** [Function](ssq_Function.html) object references the added function 


### _function_ <a id="4c484368" href="#4c484368">addFunc</a>

```cpp
inline Function addFunc (
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
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 

**return:** [Function](ssq_Function.html) object references the added function 


### _function_ <a id="bb85e908" href="#bb85e908">addFunc</a>

```cpp
inline Function addFunc (
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
* _[RuntimeException](ssq_RuntimeException.html):_ if [VM](ssq_VM.html) is invalid 

**return:** [Function](ssq_Function.html) object references the added function 


### _function_ <a id="5038ab76" href="#5038ab76">addVar</a>

```cpp
inline void addVar (
    const std::string & name,
    V T::* ptr,
    bool isStatic = false
) 
```



### _function_ <a id="f977f9bf" href="#f977f9bf">addConstVar</a>

```cpp
inline void addConstVar (
    const std::string & name,
    V T::* ptr,
    bool isStatic = false
) 
```



### _function_ <a id="4b36c7c3" href="#4b36c7c3">operator=</a>

```cpp
Class & operator= (
    const Class & other
) 
```

Copy assingment operator. 

### _function_ <a id="bf272d20" href="#bf272d20">operator=</a>

```cpp
Class & operator= (
    Class && other
) 
```

Move assingment operator. 



## Protected Functions Documentation

### _function_ <a id="21a84198" href="#21a84198">findTable</a>

```cpp
void findTable (
    const char * name,
    Object & table,
    SQFUNCTION dlg
) const 
```



### _function_ <a id="6ee153ab" href="#6ee153ab">bindVar</a>

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

### _function_ <a id="223cda5a" href="#223cda5a">dlgGetStub</a>

```cpp
static SQInteger dlgGetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="4a11e24a" href="#4a11e24a">dlgSetStub</a>

```cpp
static SQInteger dlgSetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="35272128" href="#35272128">varGetStub</a>

```cpp
static inline SQInteger varGetStub (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="1c7c3db5" href="#1c7c3db5">varSetStub</a>

```cpp
static inline SQInteger varSetStub (
    HSQUIRRELVM vm
) 
```





