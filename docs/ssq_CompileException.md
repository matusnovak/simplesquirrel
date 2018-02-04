CompileException
===================================

Compile exception thrown during compilation. 

**Inherits from:** [ssq::Exception](ssq_Exception.html)

The documentation for this class was generated from: `include/simplesquirrel/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [CompileException](#a8b8afe1) (const char * _msg_)  |
|   | [CompileException](#534230e3) (const char * _msg_, const char * _source_, int _line_, int _column_)  |
|  virtual const char * | [what](#c2b46ddc) () const  |


## Public Functions Documentation

### _function_ <a id="a8b8afe1" href="#a8b8afe1">CompileException</a>

```cpp
inline  CompileException (
    const char * msg
) 
```



### _function_ <a id="534230e3" href="#534230e3">CompileException</a>

```cpp
inline  CompileException (
    const char * msg,
    const char * source,
    int line,
    int column
) 
```



### _function_ <a id="c2b46ddc" href="#c2b46ddc">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/ssq_Exception.md#fe284d32) from class [Exception](/docs/ssq_Exception.md)



