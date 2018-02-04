Function
===================================

Squirrel function. 

**Inherits from:** [ssq::Object](ssq_Object.html)

The documentation for this class was generated from: `include/simplesquirrel/function.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|  explicit  | [Function](#595177aa) (HSQUIRRELVM _vm_)  _Constructs empty function object._ |
|  virtual  | [~Function](#bcbd9abf) ()  _Destructor._ |
|  explicit  | [Function](#2c42a9c9) (const [Object](ssq_Object.html) & _object_)  _Converts Object to Function._ |
|   | [Function](#d1490483) (const [Function](ssq_Function.html) & _other_)  _Copy constructor._ |
|   | [Function](#495f5e7b) ([Function](ssq_Function.html) && _other_)  _Move constructor._ |
|  unsigned int | [getNumOfParams](#5cd66121) () const  _Returns the number of parameters needed by the function._ |
|  [Function](ssq_Function.html) & | [operator=](#c4da652b) (const [Function](ssq_Function.html) & _other_)  _Copy assingment operator._ |
|  [Function](ssq_Function.html) & | [operator=](#8c0cbc97) ([Function](ssq_Function.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="595177aa" href="#595177aa">Function</a>

```cpp
explicit  Function (
    HSQUIRRELVM vm
) 
```

Constructs empty function object. 

### _function_ <a id="bcbd9abf" href="#bcbd9abf">~Function</a>

```cpp
virtual  ~Function () 
```

Destructor. 

### _function_ <a id="2c42a9c9" href="#2c42a9c9">Function</a>

```cpp
explicit  Function (
    const Object & object
) 
```

Converts [Object](ssq_Object.html) to [Function](ssq_Function.html). 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of a function 


### _function_ <a id="d1490483" href="#d1490483">Function</a>

```cpp
 Function (
    const Function & other
) 
```

Copy constructor. 

### _function_ <a id="495f5e7b" href="#495f5e7b">Function</a>

```cpp
 Function (
    Function && other
) 
```

Move constructor. 

### _function_ <a id="5cd66121" href="#5cd66121">getNumOfParams</a>

```cpp
unsigned int getNumOfParams () const 
```

Returns the number of parameters needed by the function. 

**note:** This ignores the "this" pointer 


### _function_ <a id="c4da652b" href="#c4da652b">operator=</a>

```cpp
Function & operator= (
    const Function & other
) 
```

Copy assingment operator. 

### _function_ <a id="8c0cbc97" href="#8c0cbc97">operator=</a>

```cpp
Function & operator= (
    Function && other
) 
```

Move assingment operator. 



