ssq
===================================


The documentation for this namespace was generated from: `include/simplesquirrel/allocators.hpp`



## Classes

| Name |
|:-----|
| class [ssq::Array](ssq_Array.html) <span style="opacity:0.8;">Squirrel intance of array object. </span> |
| class [ssq::Class](ssq_Class.html) <span style="opacity:0.8;">Squirrel class object. </span> |
| class [ssq::CompileException](ssq_CompileException.html) <span style="opacity:0.8;">Compile exception thrown during compilation. </span> |
| class [ssq::Enum](ssq_Enum.html) <span style="opacity:0.8;">Squirrel table object. </span> |
| class [ssq::Exception](ssq_Exception.html) <span style="opacity:0.8;">Raw exception. </span> |
| class [ssq::Function](ssq_Function.html) <span style="opacity:0.8;">Squirrel function. </span> |
| class [ssq::Instance](ssq_Instance.html) <span style="opacity:0.8;">Squirrel intance of class object. </span> |
| class [ssq::Libs](ssq_Libs.html) |
| class [ssq::NotFoundException](ssq_NotFoundException.html) <span style="opacity:0.8;">Not Found exception thrown if object with a given name does not exist. </span> |
| class [ssq::Object](ssq_Object.html) <span style="opacity:0.8;">Raw Squirrel object. </span> |
| class [ssq::RuntimeException](ssq_RuntimeException.html) <span style="opacity:0.8;">Runtime exception thrown if something went wrong during execution. </span> |
| class [ssq::Script](ssq_Script.html) <span style="opacity:0.8;">Squirrel script object. </span> |
| class [ssq::SqWeakRef](ssq_SqWeakRef.html) <span style="opacity:0.8;">Weak reference class that does not extend the life of the instance. </span> |
| class [ssq::Table](ssq_Table.html) <span style="opacity:0.8;">Squirrel table object. </span> |
| class [ssq::TypeException](ssq_TypeException.html) <span style="opacity:0.8;">Type exception thrown if casting between squirrel and C++ objects failed. </span> |
| class [ssq::VM](ssq_VM.html) <span style="opacity:0.8;">Squirrel Virtual Machine object. </span> |


## Enums

| Type | Name |
| -------: | :------- |
| enum | [Type](#93c564f7) |


## Typedefs

| Type | Name |
| -------: | :------- |
| typedef void(* | [SqPrintFunc](#7a2cba89) |
| typedef void(* | [SqErrorFunc](#d53adeda) |
| typedef SQInteger(* | [SqRuntimeErrorFunc](#7db87164) |
| typedef void(* | [SqCompileErrorFunc](#c5d6720d) |


## Functions

| Type | Name |
| -------: | :------- |
|  SSQ_API const char * | [typeToStr](#f6451638) (Type _type_)  |


## Enums Documentation

### _enum_ <a id="93c564f7" href="#93c564f7">Type</a>

```cpp
enum Type {
    NULLPTR,
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    TABLE,
    ARRAY,
    USERDATA,
    CLOSURE,
    NATIVECLOSURE,
    GENERATOR,
    USERPOINTER,
    THREAD,
    FUNCPROTO,
    CLASS,
    INSTANCE,
    WEAKREF,
    OUTER,
}
```





## Typedefs Documentation

### _typedef_ <a id="7a2cba89" href="#7a2cba89">SqPrintFunc</a>

```cpp
void(* SqPrintFunc
```



### _typedef_ <a id="d53adeda" href="#d53adeda">SqErrorFunc</a>

```cpp
void(* SqErrorFunc
```



### _typedef_ <a id="7db87164" href="#7db87164">SqRuntimeErrorFunc</a>

```cpp
SQInteger(* SqRuntimeErrorFunc
```



### _typedef_ <a id="c5d6720d" href="#c5d6720d">SqCompileErrorFunc</a>

```cpp
void(* SqCompileErrorFunc
```





## Functions Documentation

### _function_ <a id="f6451638" href="#f6451638">typeToStr</a>

```cpp
SSQ_API const char * typeToStr (
    Type type
) 
```





