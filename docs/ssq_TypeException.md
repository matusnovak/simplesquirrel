TypeException
===================================

Type exception thrown if casting between squirrel and C++ objects failed. 

**Inherits from:** [ssq::Exception](ssq_Exception.html)

The documentation for this class was generated from: `include/simplesquirrel/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [TypeException](#e6dfd23f) (const char * _msg_)  |
|   | [TypeException](#b3ed83b0) (const char * _msg_, const char * _expected_, const char * _got_)  |
|  virtual const char * | [what](#eee9b532) () const  |


## Public Functions Documentation

### _function_ <a id="e6dfd23f" href="#e6dfd23f">TypeException</a>

```cpp
inline  TypeException (
    const char * msg
) 
```



### _function_ <a id="b3ed83b0" href="#b3ed83b0">TypeException</a>

```cpp
inline  TypeException (
    const char * msg,
    const char * expected,
    const char * got
) 
```



### _function_ <a id="eee9b532" href="#eee9b532">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/ssq_Exception.md#fe284d32) from class [Exception](/docs/ssq_Exception.md)



