SqTypeException
===================================

Type exception thrown if casting between squirrel and C++ objects failed. 

**Inherits from:** [SquirrelBind::SqException](SquirrelBind_SqException.html)

The documentation for this class was generated from: `include/squirrelbind/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqTypeException](#e2eb24e5) (const char * _msg_)  |
|   | [SqTypeException](#a2b5e1ed) (const char * _msg_, const char * _expected_, const char * _got_)  |
|  virtual const char * | [what](#c67f5446) () const  |


## Public Functions Documentation

### _function_ <a id="e2eb24e5" href="#e2eb24e5">SqTypeException</a>

```cpp
inline  SqTypeException (
    const char * msg
) 
```



### _function_ <a id="a2b5e1ed" href="#a2b5e1ed">SqTypeException</a>

```cpp
inline  SqTypeException (
    const char * msg,
    const char * expected,
    const char * got
) 
```



### _function_ <a id="c67f5446" href="#c67f5446">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/SquirrelBind_SqException.md#6117f924) from class [SqException](/docs/SquirrelBind_SqException.md)



