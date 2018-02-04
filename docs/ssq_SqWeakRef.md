SqWeakRef
===================================

Weak reference class that does not extend the life of the instance. 

**Inherits from:** [ssq::Instance](ssq_Instance.html)

The documentation for this class was generated from: `include/simplesquirrel/instance.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqWeakRef](#41cdc15a) ()  |
|   | [SqWeakRef](#1586e624) (HSQUIRRELVM _vm_)  |
|   | [SqWeakRef](#df5c2b9a) (const [SqWeakRef](ssq_SqWeakRef.html) & _other_)  |
|   | [SqWeakRef](#eefc7cc2) ([SqWeakRef](ssq_SqWeakRef.html) && _other_)  |
|  explicit  | [SqWeakRef](#e21e9d0f) (const [Instance](ssq_Instance.html) & _instance_)  |
|  void | [swap](#0cee58dd) ([SqWeakRef](ssq_SqWeakRef.html) & _other_)  |
|  [SqWeakRef](ssq_SqWeakRef.html) & | [operator=](#4adcc221) (const [SqWeakRef](ssq_SqWeakRef.html) & _other_)  |
|  [SqWeakRef](ssq_SqWeakRef.html) & | [operator=](#1fa58b20) ([SqWeakRef](ssq_SqWeakRef.html) && _other_)  |


## Public Functions Documentation

### _function_ <a id="41cdc15a" href="#41cdc15a">SqWeakRef</a>

```cpp
 SqWeakRef () 
```



### _function_ <a id="1586e624" href="#1586e624">SqWeakRef</a>

```cpp
 SqWeakRef (
    HSQUIRRELVM vm
) 
```



### _function_ <a id="df5c2b9a" href="#df5c2b9a">SqWeakRef</a>

```cpp
 SqWeakRef (
    const SqWeakRef & other
) 
```



### _function_ <a id="eefc7cc2" href="#eefc7cc2">SqWeakRef</a>

```cpp
 SqWeakRef (
    SqWeakRef && other
) 
```



### _function_ <a id="e21e9d0f" href="#e21e9d0f">SqWeakRef</a>

```cpp
explicit  SqWeakRef (
    const Instance & instance
) 
```



### _function_ <a id="0cee58dd" href="#0cee58dd">swap</a>

```cpp
void swap (
    SqWeakRef & other
) 
```



### _function_ <a id="4adcc221" href="#4adcc221">operator=</a>

```cpp
SqWeakRef & operator= (
    const SqWeakRef & other
) 
```



### _function_ <a id="1fa58b20" href="#1fa58b20">operator=</a>

```cpp
SqWeakRef & operator= (
    SqWeakRef && other
) 
```





