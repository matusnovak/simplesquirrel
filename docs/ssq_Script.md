Script
===================================

Squirrel script object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

The documentation for this class was generated from: `include/simplesquirrel/script.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Script](#cf19fd5c) (HSQUIRRELVM _vm_)  _Creates empty script object._ |
|  virtual  | [~Script](#7c4fccee) ()  _Destructor._ |
|  void | [swap](#134b5f84) ([Script](ssq_Script.html) & _other_)  _Swaps two objects._ |
|   | [Script](#65863c8c) (const [Script](ssq_Script.html) & _other_) = delete  _Deleted copy constructor._ |
|   | [Script](#b16e58cc) ([Script](ssq_Script.html) && _other_)  _Move constructor._ |
|  [Script](ssq_Script.html) & | [operator=](#3891bf27) (const [Script](ssq_Script.html) & _other_) = delete  _Deleted copy assignment operator._ |
|  [Script](ssq_Script.html) & | [operator=](#999c5f9b) ([Script](ssq_Script.html) && _other_)  _Move assignment operator._ |


## Public Functions Documentation

### _function_ <a id="cf19fd5c" href="#cf19fd5c">Script</a>

```cpp
 Script (
    HSQUIRRELVM vm
) 
```

Creates empty script object. 

### _function_ <a id="7c4fccee" href="#7c4fccee">~Script</a>

```cpp
virtual  ~Script () 
```

Destructor. 

### _function_ <a id="134b5f84" href="#134b5f84">swap</a>

```cpp
void swap (
    Script & other
) 
```

Swaps two objects. 

### _function_ <a id="65863c8c" href="#65863c8c">Script</a>

```cpp
 Script (
    const Script & other
) = delete 
```

Deleted copy constructor. 

### _function_ <a id="b16e58cc" href="#b16e58cc">Script</a>

```cpp
 Script (
    Script && other
) 
```

Move constructor. 

### _function_ <a id="3891bf27" href="#3891bf27">operator=</a>

```cpp
Script & operator= (
    const Script & other
) = delete 
```

Deleted copy assignment operator. 

### _function_ <a id="999c5f9b" href="#999c5f9b">operator=</a>

```cpp
Script & operator= (
    Script && other
) 
```

Move assignment operator. 



