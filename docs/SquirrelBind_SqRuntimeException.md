SqRuntimeException
===================================

Runtime exception thrown if something went wrong during execution. 

**Inherits from:** [SquirrelBind::SqException](SquirrelBind_SqException.html)

The documentation for this class was generated from: `include/squirrelbind/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqRuntimeException](#98a785de) (const char * _msg_)  |
|   | [SqRuntimeException](#db999e6d) (const char * _msg_, const char * _source_, const char * _func_, int _line_)  |
|  virtual const char * | [what](#c3ec8fda) () const  |


## Public Functions Documentation

### _function_ <a id="98a785de" href="#98a785de">SqRuntimeException</a>

```cpp
inline  SqRuntimeException (
    const char * msg
) 
```



### _function_ <a id="db999e6d" href="#db999e6d">SqRuntimeException</a>

```cpp
inline  SqRuntimeException (
    const char * msg,
    const char * source,
    const char * func,
    int line
) 
```



### _function_ <a id="c3ec8fda" href="#c3ec8fda">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/SquirrelBind_SqException.md#6117f924) from class [SqException](/docs/SquirrelBind_SqException.md)



