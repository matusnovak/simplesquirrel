SqFunction
===================================

Squirrel function. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

The documentation for this class was generated from: `include/squirrelbind/function.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|  explicit  | [SqFunction](#f4d31e08) (HSQUIRRELVM _vm_)  _Constructs empty function object._ |
|  explicit  | [SqFunction](#d49b9e4a) (const [SqObject](SquirrelBind_SqObject.html) & _object_)  _Converts SqObject to SqFunction._ |
|   | [SqFunction](#52719b3c) (const [SqFunction](SquirrelBind_SqFunction.html) & _other_)  _Copy constructor._ |
|   | [SqFunction](#2f74b37f) ([SqFunction](SquirrelBind_SqFunction.html) && _other_)  _Move constructor._ |
|  unsigned int | [getNumOfParams](#f3ab091a) () const  _Returns the number of parameters needed by the function._ |
|  [SqFunction](SquirrelBind_SqFunction.html) & | [operator=](#21c57d22) (const [SqFunction](SquirrelBind_SqFunction.html) & _other_)  _Copy assingment operator._ |
|  [SqFunction](SquirrelBind_SqFunction.html) & | [operator=](#ad5b95b6) ([SqFunction](SquirrelBind_SqFunction.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="f4d31e08" href="#f4d31e08">SqFunction</a>

```cpp
explicit  SqFunction (
    HSQUIRRELVM vm
) 
```

Constructs empty function object. 

### _function_ <a id="d49b9e4a" href="#d49b9e4a">SqFunction</a>

```cpp
explicit  SqFunction (
    const SqObject & object
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to [SqFunction](SquirrelBind_SqFunction.html). 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of a function 


### _function_ <a id="52719b3c" href="#52719b3c">SqFunction</a>

```cpp
 SqFunction (
    const SqFunction & other
) 
```

Copy constructor. 

### _function_ <a id="2f74b37f" href="#2f74b37f">SqFunction</a>

```cpp
 SqFunction (
    SqFunction && other
) 
```

Move constructor. 

### _function_ <a id="f3ab091a" href="#f3ab091a">getNumOfParams</a>

```cpp
unsigned int getNumOfParams () const 
```

Returns the number of parameters needed by the function. 

**note:** This ignores the "this" pointer 


### _function_ <a id="21c57d22" href="#21c57d22">operator=</a>

```cpp
SqFunction & operator= (
    const SqFunction & other
) 
```

Copy assingment operator. 

### _function_ <a id="ad5b95b6" href="#ad5b95b6">operator=</a>

```cpp
SqFunction & operator= (
    SqFunction && other
) 
```

Move assingment operator. 



