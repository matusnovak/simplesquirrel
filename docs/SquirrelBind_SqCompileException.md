SqCompileException
===================================

Compile exception thrown during compilation. 

**Inherits from:** [SquirrelBind::SqException](SquirrelBind_SqException.html)

The documentation for this class was generated from: `include/squirrelbind/exceptions.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqCompileException](#bad7a29a) (const char * _msg_)  |
|   | [SqCompileException](#90ec4df9) (const char * _msg_, const char * _source_, int _line_, int _column_)  |
|  virtual const char * | [what](#ff24b0bf) () const  |


## Public Functions Documentation

### _function_ <a id="bad7a29a" href="#bad7a29a">SqCompileException</a>

```cpp
inline  SqCompileException (
    const char * msg
) 
```



### _function_ <a id="90ec4df9" href="#90ec4df9">SqCompileException</a>

```cpp
inline  SqCompileException (
    const char * msg,
    const char * source,
    int line,
    int column
) 
```



### _function_ <a id="ff24b0bf" href="#ff24b0bf">what</a>

```cpp
inline virtual const char * what () const 
```



**Overrides:** [what](/docs/SquirrelBind_SqException.md#6117f924) from class [SqException](/docs/SquirrelBind_SqException.md)



