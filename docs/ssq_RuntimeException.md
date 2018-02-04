RuntimeException
===================================

Runtime exception thrown if something went wrong during execution. 

**Inherits from:** [ssq::Exception](ssq_Exception.html)

The documentation for this class was generated from: `include/simplesquirrel/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [RuntimeException](#8430579e) (const char * _msg_)  |
|   | [RuntimeException](#68519589) (const char * _msg_, const char * _source_, const char * _func_, int _line_)  |
|  virtual const char * | [what](#2c5e367c) () const  |


## Public Functions Documentation

### _function_ <a id="8430579e" href="#8430579e">RuntimeException</a>

```cpp
inline  RuntimeException (
    const char * msg
) 
```



### _function_ <a id="68519589" href="#68519589">RuntimeException</a>

```cpp
inline  RuntimeException (
    const char * msg,
    const char * source,
    const char * func,
    int line
) 
```



### _function_ <a id="2c5e367c" href="#2c5e367c">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/ssq_Exception.md#fe284d32) from class [Exception](/docs/ssq_Exception.md)



