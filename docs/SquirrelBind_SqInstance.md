SqInstance
===================================

Squirrel intance of class object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

**Implemented by:** [SquirrelBind::SqWeakRef](SquirrelBind_SqWeakRef.html)

The documentation for this class was generated from: `include/squirrelbind/instance.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqInstance](#cc2be9c4) ()  _Constructs empty invalid instance._ |
|   | [SqInstance](#6eaf701e) (HSQUIRRELVM _vm_)  _Constructs empty instance._ |
|  explicit  | [SqInstance](#35e56723) (const [SqObject](SquirrelBind_SqObject.html) & _object_)  _Converts SqObject to SqInstance._ |
|   | [SqInstance](#efb324d0) (const [SqInstance](SquirrelBind_SqInstance.html) & _other_)  _Copy constructor._ |
|   | [SqInstance](#3a3a2855) ([SqInstance](SquirrelBind_SqInstance.html) && _other_)  _Move constructor._ |
|  [SqClass](SquirrelBind_SqClass.html) | [getClass](#bd201764) ()  _Returns the class associated with this instance._ |
|  [SqInstance](SquirrelBind_SqInstance.html) & | [operator=](#b8bc6b2b) (const [SqInstance](SquirrelBind_SqInstance.html) & _other_)  _Copy assingment operator._ |
|  [SqInstance](SquirrelBind_SqInstance.html) & | [operator=](#51c9c5b9) ([SqInstance](SquirrelBind_SqInstance.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="cc2be9c4" href="#cc2be9c4">SqInstance</a>

```cpp
 SqInstance () 
```

Constructs empty invalid instance. 

### _function_ <a id="6eaf701e" href="#6eaf701e">SqInstance</a>

```cpp
 SqInstance (
    HSQUIRRELVM vm
) 
```

Constructs empty instance. 

### _function_ <a id="35e56723" href="#35e56723">SqInstance</a>

```cpp
explicit  SqInstance (
    const SqObject & object
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to [SqInstance](SquirrelBind_SqInstance.html). 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of an instance 


### _function_ <a id="efb324d0" href="#efb324d0">SqInstance</a>

```cpp
 SqInstance (
    const SqInstance & other
) 
```

Copy constructor. 

### _function_ <a id="3a3a2855" href="#3a3a2855">SqInstance</a>

```cpp
 SqInstance (
    SqInstance && other
) 
```

Move constructor. 

### _function_ <a id="bd201764" href="#bd201764">getClass</a>

```cpp
SqClass getClass () 
```

Returns the class associated with this instance. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if something went wrong 


### _function_ <a id="b8bc6b2b" href="#b8bc6b2b">operator=</a>

```cpp
SqInstance & operator= (
    const SqInstance & other
) 
```

Copy assingment operator. 

### _function_ <a id="51c9c5b9" href="#51c9c5b9">operator=</a>

```cpp
SqInstance & operator= (
    SqInstance && other
) 
```

Move assingment operator. 



