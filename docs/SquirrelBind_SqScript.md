SqScript
===================================

Squirrel script object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

The documentation for this class was generated from: `include/squirrelbind/script.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqScript](#0562db4a) (HSQUIRRELVM _vm_)  _Creates empty script object._ |
|  void | [swap](#226bb698) ([SqScript](SquirrelBind_SqScript.html) & _other_)  _Swaps two objects._ |
|   | [SqScript](#00c8ea7e) (const [SqScript](SquirrelBind_SqScript.html) & _other_) = delete  _Deleted copy constructor._ |
|   | [SqScript](#6bbc3b76) ([SqScript](SquirrelBind_SqScript.html) && _other_)  _Move constructor._ |
|  [SqScript](SquirrelBind_SqScript.html) & | [operator=](#a23527e3) (const [SqScript](SquirrelBind_SqScript.html) & _other_) = delete  _Deleted copy assignment operator._ |
|  [SqScript](SquirrelBind_SqScript.html) & | [operator=](#2e52835c) ([SqScript](SquirrelBind_SqScript.html) && _other_)  _Move assignment operator._ |


## Public Functions Documentation

### _function_ <a id="0562db4a" href="#0562db4a">SqScript</a>

```cpp
 SqScript (
    HSQUIRRELVM vm
) 
```

Creates empty script object. 

### _function_ <a id="226bb698" href="#226bb698">swap</a>

```cpp
void swap (
    SqScript & other
) 
```

Swaps two objects. 

### _function_ <a id="00c8ea7e" href="#00c8ea7e">SqScript</a>

```cpp
 SqScript (
    const SqScript & other
) = delete 
```

Deleted copy constructor. 

### _function_ <a id="6bbc3b76" href="#6bbc3b76">SqScript</a>

```cpp
 SqScript (
    SqScript && other
) 
```

Move constructor. 

### _function_ <a id="a23527e3" href="#a23527e3">operator=</a>

```cpp
SqScript & operator= (
    const SqScript & other
) = delete 
```

Deleted copy assignment operator. 

### _function_ <a id="2e52835c" href="#2e52835c">operator=</a>

```cpp
SqScript & operator= (
    SqScript && other
) 
```

Move assignment operator. 



