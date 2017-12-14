SquirrelBind
===================================


The documentation for this namespace was generated from: `include/squirrelbind/allocators.hpp`



## Classes

| Name |
|:-----|
| class [SquirrelBind::SqArray](SquirrelBind_SqArray.html) <span style="opacity:0.8;">Squirrel intance of array object. </span> |
| class [SquirrelBind::SqClass](SquirrelBind_SqClass.html) <span style="opacity:0.8;">Squirrel class object. </span> |
| class [SquirrelBind::SqCompileException](SquirrelBind_SqCompileException.html) <span style="opacity:0.8;">Compile exception thrown during compilation. </span> |
| class [SquirrelBind::SqEnum](SquirrelBind_SqEnum.html) <span style="opacity:0.8;">Squirrel table object. </span> |
| class [SquirrelBind::SqException](SquirrelBind_SqException.html) <span style="opacity:0.8;">Raw exception. </span> |
| class [SquirrelBind::SqFunction](SquirrelBind_SqFunction.html) <span style="opacity:0.8;">Squirrel function. </span> |
| class [SquirrelBind::SqInstance](SquirrelBind_SqInstance.html) <span style="opacity:0.8;">Squirrel intance of class object. </span> |
| class [SquirrelBind::SqLibs](SquirrelBind_SqLibs.html) |
| class [SquirrelBind::SqNotFoundException](SquirrelBind_SqNotFoundException.html) <span style="opacity:0.8;">Not Found exception thrown if object with a given name does not exist. </span> |
| class [SquirrelBind::SqObject](SquirrelBind_SqObject.html) <span style="opacity:0.8;">Raw Squirrel object. </span> |
| class [SquirrelBind::SqRuntimeException](SquirrelBind_SqRuntimeException.html) <span style="opacity:0.8;">Runtime exception thrown if something went wrong during execution. </span> |
| class [SquirrelBind::SqScript](SquirrelBind_SqScript.html) <span style="opacity:0.8;">Squirrel script object. </span> |
| class [SquirrelBind::SqTable](SquirrelBind_SqTable.html) <span style="opacity:0.8;">Squirrel table object. </span> |
| class [SquirrelBind::SqTypeException](SquirrelBind_SqTypeException.html) <span style="opacity:0.8;">Type exception thrown if casting between squirrel and C++ objects failed. </span> |
| class [SquirrelBind::SqVM](SquirrelBind_SqVM.html) <span style="opacity:0.8;">Squirrel Virtual Machine object. </span> |
| class [SquirrelBind::SqWeakRef](SquirrelBind_SqWeakRef.html) <span style="opacity:0.8;">Weak reference class that does not extend the life of the instance. </span> |


## Enums

| Type | Name |
| -------: | :------- |
| enum | [SqType](#c430f06a) |


## Typedefs

| Type | Name |
| -------: | :------- |
| typedef void(* | [SqPrintFunc](#36998101) |
| typedef void(* | [SqErrorFunc](#8de85840) |
| typedef SQInteger(* | [SqRuntimeErrorFunc](#4d456308) |
| typedef void(* | [SqCompileErrorFunc](#356b6c86) |


## Functions

| Type | Name |
| -------: | :------- |
|  SQBIND_API const char * | [sqTypeToStr](#693f5011) (SqType _type_)  |


## Enums Documentation

### _enum_ <a id="c430f06a" href="#c430f06a">SqType</a>

```cpp
enum SqType {
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

### _typedef_ <a id="36998101" href="#36998101">SqPrintFunc</a>

```cpp
void(* SqPrintFunc
```



### _typedef_ <a id="8de85840" href="#8de85840">SqErrorFunc</a>

```cpp
void(* SqErrorFunc
```



### _typedef_ <a id="4d456308" href="#4d456308">SqRuntimeErrorFunc</a>

```cpp
SQInteger(* SqRuntimeErrorFunc
```



### _typedef_ <a id="356b6c86" href="#356b6c86">SqCompileErrorFunc</a>

```cpp
void(* SqCompileErrorFunc
```





## Functions Documentation

### _function_ <a id="693f5011" href="#693f5011">sqTypeToStr</a>

```cpp
SQBIND_API const char * sqTypeToStr (
    SqType type
) 
```





