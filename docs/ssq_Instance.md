Instance
===================================

Squirrel intance of class object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

**Implemented by:** [ssq::SqWeakRef](ssq_SqWeakRef.html)

The documentation for this class was generated from: `include/simplesquirrel/instance.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Instance](#b41af8a5) ()  _Constructs empty invalid instance._ |
|  virtual  | [~Instance](#51cc576c) ()  _Destructor._ |
|   | [Instance](#eb3f50bc) (HSQUIRRELVM _vm_)  _Constructs empty instance._ |
|  explicit  | [Instance](#d77bf014) (const [Object](ssq_Object.html) & _object_)  _Converts Object to Instance._ |
|   | [Instance](#c9c09f09) (const [Instance](ssq_Instance.html) & _other_)  _Copy constructor._ |
|   | [Instance](#843872fd) ([Instance](ssq_Instance.html) && _other_)  _Move constructor._ |
|  [Class](ssq_Class.html) | [getClass](#6e073bbf) ()  _Returns the class associated with this instance._ |
|  [Instance](ssq_Instance.html) & | [operator=](#e85e3e1a) (const [Instance](ssq_Instance.html) & _other_)  _Copy assingment operator._ |
|  [Instance](ssq_Instance.html) & | [operator=](#4cd45110) ([Instance](ssq_Instance.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="b41af8a5" href="#b41af8a5">Instance</a>

```cpp
 Instance () 
```

Constructs empty invalid instance. 

### _function_ <a id="51cc576c" href="#51cc576c">~Instance</a>

```cpp
virtual  ~Instance () 
```

Destructor. 

### _function_ <a id="eb3f50bc" href="#eb3f50bc">Instance</a>

```cpp
 Instance (
    HSQUIRRELVM vm
) 
```

Constructs empty instance. 

### _function_ <a id="d77bf014" href="#d77bf014">Instance</a>

```cpp
explicit  Instance (
    const Object & object
) 
```

Converts [Object](ssq_Object.html) to [Instance](ssq_Instance.html). 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of an instance 


### _function_ <a id="c9c09f09" href="#c9c09f09">Instance</a>

```cpp
 Instance (
    const Instance & other
) 
```

Copy constructor. 

### _function_ <a id="843872fd" href="#843872fd">Instance</a>

```cpp
 Instance (
    Instance && other
) 
```

Move constructor. 

### _function_ <a id="6e073bbf" href="#6e073bbf">getClass</a>

```cpp
Class getClass () 
```

Returns the class associated with this instance. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if something went wrong 


### _function_ <a id="e85e3e1a" href="#e85e3e1a">operator=</a>

```cpp
Instance & operator= (
    const Instance & other
) 
```

Copy assingment operator. 

### _function_ <a id="4cd45110" href="#4cd45110">operator=</a>

```cpp
Instance & operator= (
    Instance && other
) 
```

Move assingment operator. 



