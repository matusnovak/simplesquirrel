SqWeakRef
===================================

Weak reference class that does not extend the life of the instance. 

**Inherits from:** [SquirrelBind::SqInstance](SquirrelBind_SqInstance.html)

The documentation for this class was generated from: `include/squirrelbind/instance.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqWeakRef](#b06c3e5b) ()  |
|   | [SqWeakRef](#1feabca4) (HSQUIRRELVM _vm_)  |
|   | [SqWeakRef](#5326845b) (const [SqWeakRef](SquirrelBind_SqWeakRef.html) & _other_)  |
|   | [SqWeakRef](#227146d8) ([SqWeakRef](SquirrelBind_SqWeakRef.html) && _other_)  |
|  void | [swap](#ef95ca45) ([SqWeakRef](SquirrelBind_SqWeakRef.html) & _other_)  |
|  [SqWeakRef](SquirrelBind_SqWeakRef.html) & | [operator=](#82396a94) (const [SqWeakRef](SquirrelBind_SqWeakRef.html) & _other_)  |
|  [SqWeakRef](SquirrelBind_SqWeakRef.html) & | [operator=](#b20fad18) ([SqWeakRef](SquirrelBind_SqWeakRef.html) && _other_)  |


## Public Functions Documentation

### _function_ <a id="b06c3e5b" href="#b06c3e5b">SqWeakRef</a>

```cpp
 SqWeakRef () 
```



### _function_ <a id="1feabca4" href="#1feabca4">SqWeakRef</a>

```cpp
 SqWeakRef (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="5326845b" href="#5326845b">SqWeakRef</a>

```cpp
 SqWeakRef (
    const SqWeakRef & other
) 
```



### _function_ <a id="227146d8" href="#227146d8">SqWeakRef</a>

```cpp
 SqWeakRef (
    SqWeakRef && other
) 
```



### _function_ <a id="ef95ca45" href="#ef95ca45">swap</a>

```cpp
void swap (
    SqWeakRef & other
) 
```



### _function_ <a id="82396a94" href="#82396a94">operator=</a>

```cpp
SqWeakRef & operator= (
    const SqWeakRef & other
) 
```



### _function_ <a id="b20fad18" href="#b20fad18">operator=</a>

```cpp
SqWeakRef & operator= (
    SqWeakRef && other
) 
```





