SqArray
===================================

Squirrel intance of array object. 

**Inherits from:** [SquirrelBind::SqObject](SquirrelBind_SqObject.html)

The documentation for this class was generated from: `include/squirrelbind/array.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqArray](#d51e90b7) (HSQUIRRELVM _vm_, size_t _len_ = 0)  _Constructs empty array._ |
|   | [SqArray](#875b1b86) (HSQUIRRELVM _vm_, const std::vector< T > & _vector_)  _Constructs array out of std::vector._ |
|  explicit  | [SqArray](#4318fba5) (const [SqObject](SquirrelBind_SqObject.html) & _object_)  _Converts SqObject to SqArray._ |
|   | [SqArray](#16d1bdec) (const [SqArray](SquirrelBind_SqArray.html) & _other_)  _Copy constructor._ |
|   | [SqArray](#63c69e7b) ([SqArray](SquirrelBind_SqArray.html) && _other_)  _Move constructor._ |
|  size_t | [size](#453dbdbc) ()  _Returns the size of the array._ |
|  void | [push](#14b18406) (const T & _value_)  _Pushes an element to the back of the array._ |
|  T | [popAndGet](#ddb580b8) ()  _Pops an element from the back of the array and returns it._ |
|  void | [pop](#fdf51616) ()  _Pops an element from the back of the array._ |
|  T | [get](#2b8b0e09) (size_t _index_)  _Returns an element from the specific index._ |
|  T | [begin](#01cfba55) ()  |
|  T | [back](#398391df) ()  |
|  void | [set](#ef7caf8f) (size_t _index_, const T & _value_)  _Sets an element at the specific index._ |
|  std::vector< [SqObject](SquirrelBind_SqObject.html) > | [convertRaw](#f44019d3) ()  _Converts this array to std::vector of objects._ |
|  std::vector< T > | [convert](#c2f373b0) ()  _Converts this array to std::vector of specific type T._ |
|  [SqArray](SquirrelBind_SqArray.html) & | [operator=](#1488617b) (const [SqArray](SquirrelBind_SqArray.html) & _other_)  _Copy assingment operator._ |
|  [SqArray](SquirrelBind_SqArray.html) & | [operator=](#91eed091) ([SqArray](SquirrelBind_SqArray.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="d51e90b7" href="#d51e90b7">SqArray</a>

```cpp
 SqArray (
    HSQUIRRELVM vm,
    size_t len = 0
) 
```

Constructs empty array. 

### _function_ <a id="875b1b86" href="#875b1b86">SqArray</a>

```cpp
inline  SqArray (
    HSQUIRRELVM vm,
    const std::vector< T > & vector
) 
```

Constructs array out of std::vector. 

### _function_ <a id="4318fba5" href="#4318fba5">SqArray</a>

```cpp
explicit  SqArray (
    const SqObject & object
) 
```

Converts [SqObject](SquirrelBind_SqObject.html) to [SqArray](SquirrelBind_SqArray.html). 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the [SqObject](SquirrelBind_SqObject.html) is not type of an array 


### _function_ <a id="16d1bdec" href="#16d1bdec">SqArray</a>

```cpp
 SqArray (
    const SqArray & other
) 
```

Copy constructor. 

### _function_ <a id="63c69e7b" href="#63c69e7b">SqArray</a>

```cpp
 SqArray (
    SqArray && other
) 
```

Move constructor. 

### _function_ <a id="453dbdbc" href="#453dbdbc">size</a>

```cpp
size_t size () 
```

Returns the size of the array. 

### _function_ <a id="14b18406" href="#14b18406">push</a>

```cpp
inline void push (
    const T & value
) 
```

Pushes an element to the back of the array. 

### _function_ <a id="ddb580b8" href="#ddb580b8">popAndGet</a>

```cpp
inline T popAndGet () 
```

Pops an element from the back of the array and returns it. 

### _function_ <a id="fdf51616" href="#fdf51616">pop</a>

```cpp
void pop () 
```

Pops an element from the back of the array. 

### _function_ <a id="2b8b0e09" href="#2b8b0e09">get</a>

```cpp
inline T get (
    size_t index
) 
```

Returns an element from the specific index. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the index is out of bounds or element cannot be returned 


### _function_ <a id="01cfba55" href="#01cfba55">begin</a>

```cpp
inline T begin () 
```



Returns the element at the start of the array **Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the array is empty or element cannot be returned 


### _function_ <a id="398391df" href="#398391df">back</a>

```cpp
inline T back () 
```



Returns the element at the end of the array **Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the array is empty or element cannot be returned 


### _function_ <a id="ef7caf8f" href="#ef7caf8f">set</a>

```cpp
inline void set (
    size_t index,
    const T & value
) 
```

Sets an element at the specific index. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if the index is out of bounds or element cannot be set 


### _function_ <a id="f44019d3" href="#f44019d3">convertRaw</a>

```cpp
std::vector< SqObject > convertRaw () 
```

Converts this array to std::vector of objects. 

### _function_ <a id="c2f373b0" href="#c2f373b0">convert</a>

```cpp
inline std::vector< T > convert () 
```

Converts this array to std::vector of specific type T. 

### _function_ <a id="1488617b" href="#1488617b">operator=</a>

```cpp
SqArray & operator= (
    const SqArray & other
) 
```

Copy assingment operator. 

### _function_ <a id="91eed091" href="#91eed091">operator=</a>

```cpp
SqArray & operator= (
    SqArray && other
) 
```

Move assingment operator. 



