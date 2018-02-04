Array
===================================

Squirrel intance of array object. 

**Inherits from:** [ssq::Object](ssq_Object.html)

The documentation for this class was generated from: `include/simplesquirrel/array.hpp`



## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Array](#35684b72) (HSQUIRRELVM _vm_, size_t _len_ = 0)  _Constructs empty array._ |
|  virtual  | [~Array](#54fe11fd) ()  _Destructor._ |
|   | [Array](#5256ef2d) (HSQUIRRELVM _vm_, const std::vector< T > & _vector_)  _Constructs array out of std::vector._ |
|  explicit  | [Array](#f26619a0) (const [Object](ssq_Object.html) & _object_)  _Converts Object to Array._ |
|   | [Array](#553c2453) (const [Array](ssq_Array.html) & _other_)  _Copy constructor._ |
|   | [Array](#2044280c) ([Array](ssq_Array.html) && _other_)  _Move constructor._ |
|  size_t | [size](#f2b3a313) ()  _Returns the size of the array._ |
|  void | [push](#033cd5a7) (const T & _value_)  _Pushes an element to the back of the array._ |
|  T | [popAndGet](#f7ed5f8c) ()  _Pops an element from the back of the array and returns it._ |
|  void | [pop](#1fb68cf3) ()  _Pops an element from the back of the array._ |
|  T | [get](#a980ef9f) (size_t _index_)  _Returns an element from the specific index._ |
|  T | [begin](#2ca1de07) ()  |
|  T | [back](#cddaa438) ()  |
|  void | [set](#7fc91edd) (size_t _index_, const T & _value_)  _Sets an element at the specific index._ |
|  std::vector< [Object](ssq_Object.html) > | [convertRaw](#8c3b3aaa) ()  _Converts this array to std::vector of objects._ |
|  std::vector< T > | [convert](#ecd408dd) ()  _Converts this array to std::vector of specific type T._ |
|  [Array](ssq_Array.html) & | [operator=](#8915149d) (const [Array](ssq_Array.html) & _other_)  _Copy assingment operator._ |
|  [Array](ssq_Array.html) & | [operator=](#62b1acc6) ([Array](ssq_Array.html) && _other_)  _Move assingment operator._ |


## Public Functions Documentation

### _function_ <a id="35684b72" href="#35684b72">Array</a>

```cpp
 Array (
    HSQUIRRELVM vm,
    size_t len = 0
) 
```

Constructs empty array. 

### _function_ <a id="54fe11fd" href="#54fe11fd">~Array</a>

```cpp
virtual  ~Array () 
```

Destructor. 

### _function_ <a id="5256ef2d" href="#5256ef2d">Array</a>

```cpp
inline  Array (
    HSQUIRRELVM vm,
    const std::vector< T > & vector
) 
```

Constructs array out of std::vector. 

### _function_ <a id="f26619a0" href="#f26619a0">Array</a>

```cpp
explicit  Array (
    const Object & object
) 
```

Converts [Object](ssq_Object.html) to [Array](ssq_Array.html). 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the [Object](ssq_Object.html) is not type of an array 


### _function_ <a id="553c2453" href="#553c2453">Array</a>

```cpp
 Array (
    const Array & other
) 
```

Copy constructor. 

### _function_ <a id="2044280c" href="#2044280c">Array</a>

```cpp
 Array (
    Array && other
) 
```

Move constructor. 

### _function_ <a id="f2b3a313" href="#f2b3a313">size</a>

```cpp
size_t size () 
```

Returns the size of the array. 

### _function_ <a id="033cd5a7" href="#033cd5a7">push</a>

```cpp
inline void push (
    const T & value
) 
```

Pushes an element to the back of the array. 

### _function_ <a id="f7ed5f8c" href="#f7ed5f8c">popAndGet</a>

```cpp
inline T popAndGet () 
```

Pops an element from the back of the array and returns it. 

### _function_ <a id="1fb68cf3" href="#1fb68cf3">pop</a>

```cpp
void pop () 
```

Pops an element from the back of the array. 

### _function_ <a id="a980ef9f" href="#a980ef9f">get</a>

```cpp
inline T get (
    size_t index
) 
```

Returns an element from the specific index. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the index is out of bounds or element cannot be returned 


### _function_ <a id="2ca1de07" href="#2ca1de07">begin</a>

```cpp
inline T begin () 
```



Returns the element at the start of the array **Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the array is empty or element cannot be returned 


### _function_ <a id="cddaa438" href="#cddaa438">back</a>

```cpp
inline T back () 
```



Returns the element at the end of the array **Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the array is empty or element cannot be returned 


### _function_ <a id="7fc91edd" href="#7fc91edd">set</a>

```cpp
inline void set (
    size_t index,
    const T & value
) 
```

Sets an element at the specific index. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if the index is out of bounds or element cannot be set 


### _function_ <a id="8c3b3aaa" href="#8c3b3aaa">convertRaw</a>

```cpp
std::vector< Object > convertRaw () 
```

Converts this array to std::vector of objects. 

### _function_ <a id="ecd408dd" href="#ecd408dd">convert</a>

```cpp
inline std::vector< T > convert () 
```

Converts this array to std::vector of specific type T. 

### _function_ <a id="8915149d" href="#8915149d">operator=</a>

```cpp
Array & operator= (
    const Array & other
) 
```

Copy assingment operator. 

### _function_ <a id="62b1acc6" href="#62b1acc6">operator=</a>

```cpp
Array & operator= (
    Array && other
) 
```

Move assingment operator. 



