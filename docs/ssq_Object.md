Object
===================================

Raw Squirrel object. 

**Implemented by:** [ssq::Array](ssq_Array.html), [ssq::Class](ssq_Class.html), [ssq::Enum](ssq_Enum.html), [ssq::Function](ssq_Function.html), [ssq::Instance](ssq_Instance.html), [ssq::Script](ssq_Script.html), [ssq::Table](ssq_Table.html), 

The documentation for this class was generated from: `include/simplesquirrel/object.hpp`



## Protected Attributes

| Type | Name |
| -------: | :------- |
|  HSQUIRRELVM | [vm](#5d0dae14) |
|  HSQOBJECT | [obj](#d1245820) |
|  bool | [weak](#fb81c2e5) |


## Public Functions

| Type | Name |
| -------: | :------- |
|   | [Object](#ee95dd04) ()  _Creates an empty object with null VM._ |
|   | [Object](#0f889e49) (HSQUIRRELVM _vm_)  _Creates an empty object._ |
|  virtual  | [~Object](#dd98f2c7) ()  |
|  void | [swap](#c43e3cf9) ([Object](ssq_Object.html) & _other_)  _Swaps two objects._ |
|   | [Object](#aa5598eb) (const [Object](ssq_Object.html) & _other_)  _Copy constructor to copy the object reference._ |
|   | [Object](#c2097196) ([Object](ssq_Object.html) && _other_)  _Move constructor._ |
|  bool | [isEmpty](#2f9363bf) () const  _Checks if the object is empty._ |
|  const HSQOBJECT & | [getRaw](#269a3cbc) () const  _Returns raw Squirrel object reference._ |
|  HSQOBJECT & | [getRaw](#f2a2028b) ()  _Returns raw Squirrel object reference._ |
|  [Object](ssq_Object.html) | [find](#50cad943) (const char * _name_) const  _Finds object within this object._ |
|  Type | [getType](#b9ddac1f) () const  _Returns the type of the object._ |
|  const char * | [getTypeStr](#e2771d4e) () const  _Returns the type of the object in string format._ |
|  const HSQUIRRELVM & | [getHandle](#b064266b) () const  _Returns the Squirrel virtual machine handle associated with this instance._ |
|  size_t | [getTypeTag](#8ea23ed6) () const  _Returns the typetag associated with this object._ |
|  bool | [isNull](#d8275b03) () const  _Returns true if the object is nullptr._ |
|  void | [reset](#5bbbb478) ()  _Releases the object and resets it to empty._ |
|  int32_t | [toInt](#2b2a31a8) () const  _Returns the integer value of this object._ |
|  float | [toFloat](#e8908d0f) () const  _Returns the float value of this object._ |
|  std::string | [toString](#a85bb5d7) () const  _Returns the string value of this object._ |
|  bool | [toBool](#190f8636) () const  _Returns the boolean value of this object._ |
|  [Function](ssq_Function.html) | [toFunction](#201d32d5) () const  _Returns the Function value of this object._ |
|  [Instance](ssq_Instance.html) | [toInstance](#c9da9d23) () const  _Returns the Instance value of this object._ |
|  [Class](ssq_Class.html) | [toClass](#a9045351) () const  _Returns the Class value of this object._ |
|  [Table](ssq_Table.html) | [toTable](#73161c05) () const  _Returns the Table value of this object._ |
|  [Array](ssq_Array.html) | [toArray](#f32428a8) () const  _Returns the Array value of this object._ |
|  T | [to](#6842bdfa) () const  _Returns an arbitary value of this object._ |
|  T | [toPtrUnsafe](#17bcc29e) () const  _Unsafe cast this object into any pointer of type T._ |
|  [Object](ssq_Object.html) & | [operator=](#54c8dbb1) (const [Object](ssq_Object.html) & _other_)  _Copy assingment operator._ |
|  [Object](ssq_Object.html) & | [operator=](#6272d602) ([Object](ssq_Object.html) && _other_)  _Move assingment operator._ |


## Protected Attributes Documentation

### _variable_ <a id="5d0dae14" href="#5d0dae14">vm</a>

```cpp
HSQUIRRELVM vm
```



### _variable_ <a id="d1245820" href="#d1245820">obj</a>

```cpp
HSQOBJECT obj
```



### _variable_ <a id="fb81c2e5" href="#fb81c2e5">weak</a>

```cpp
bool weak
```





## Public Functions Documentation

### _function_ <a id="ee95dd04" href="#ee95dd04">Object</a>

```cpp
 Object () 
```

Creates an empty object with null [VM](ssq_VM.html). 

**note:** This object won't be usable 


### _function_ <a id="0f889e49" href="#0f889e49">Object</a>

```cpp
 Object (
    HSQUIRRELVM vm
) 
```

Creates an empty object. 

### _function_ <a id="dd98f2c7" href="#dd98f2c7">~Object</a>

```cpp
virtual  ~Object () 
```



### _function_ <a id="c43e3cf9" href="#c43e3cf9">swap</a>

```cpp
void swap (
    Object & other
) 
```

Swaps two objects. 

### _function_ <a id="aa5598eb" href="#aa5598eb">Object</a>

```cpp
 Object (
    const Object & other
) 
```

Copy constructor to copy the object reference. 

### _function_ <a id="c2097196" href="#c2097196">Object</a>

```cpp
 Object (
    Object && other
) 
```

Move constructor. 

### _function_ <a id="2f9363bf" href="#2f9363bf">isEmpty</a>

```cpp
bool isEmpty () const 
```

Checks if the object is empty. 

### _function_ <a id="269a3cbc" href="#269a3cbc">getRaw</a>

```cpp
const HSQOBJECT & getRaw () const 
```

Returns raw Squirrel object reference. 

### _function_ <a id="f2a2028b" href="#f2a2028b">getRaw</a>

```cpp
HSQOBJECT & getRaw () 
```

Returns raw Squirrel object reference. 

### _function_ <a id="50cad943" href="#50cad943">find</a>

```cpp
Object find (
    const char * name
) const 
```

Finds object within this object. 

### _function_ <a id="b9ddac1f" href="#b9ddac1f">getType</a>

```cpp
Type getType () const 
```

Returns the type of the object. 

### _function_ <a id="e2771d4e" href="#e2771d4e">getTypeStr</a>

```cpp
const char * getTypeStr () const 
```

Returns the type of the object in string format. 

### _function_ <a id="b064266b" href="#b064266b">getHandle</a>

```cpp
const HSQUIRRELVM & getHandle () const 
```

Returns the Squirrel virtual machine handle associated with this instance. 

### _function_ <a id="8ea23ed6" href="#8ea23ed6">getTypeTag</a>

```cpp
size_t getTypeTag () const 
```

Returns the typetag associated with this object. 

**note:** The typetag is equal to hash value of typeid(T) 


### _function_ <a id="d8275b03" href="#d8275b03">isNull</a>

```cpp
bool isNull () const 
```

Returns true if the object is nullptr. 

### _function_ <a id="5bbbb478" href="#5bbbb478">reset</a>

```cpp
void reset () 
```

Releases the object and resets it to empty. 

### _function_ <a id="2b2a31a8" href="#2b2a31a8">toInt</a>

```cpp
int32_t toInt () const 
```

Returns the integer value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not an integer 


### _function_ <a id="e8908d0f" href="#e8908d0f">toFloat</a>

```cpp
float toFloat () const 
```

Returns the float value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a float 


### _function_ <a id="a85bb5d7" href="#a85bb5d7">toString</a>

```cpp
std::string toString () const 
```

Returns the string value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a sring 


### _function_ <a id="190f8636" href="#190f8636">toBool</a>

```cpp
bool toBool () const 
```

Returns the boolean value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a boolean 


### _function_ <a id="201d32d5" href="#201d32d5">toFunction</a>

```cpp
Function toFunction () const 
```

Returns the [Function](ssq_Function.html) value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a function 


### _function_ <a id="c9da9d23" href="#c9da9d23">toInstance</a>

```cpp
Instance toInstance () const 
```

Returns the [Instance](ssq_Instance.html) value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not an instance 


### _function_ <a id="a9045351" href="#a9045351">toClass</a>

```cpp
Class toClass () const 
```

Returns the [Class](ssq_Class.html) value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a class 


### _function_ <a id="73161c05" href="#73161c05">toTable</a>

```cpp
Table toTable () const 
```

Returns the [Table](ssq_Table.html) value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not a table 


### _function_ <a id="f32428a8" href="#f32428a8">toArray</a>

```cpp
Array toArray () const 
```

Returns the [Array](ssq_Array.html) value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not an array 


### _function_ <a id="6842bdfa" href="#6842bdfa">to</a>

```cpp
T to () const 
```

Returns an arbitary value of this object. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not an type of T 


### _function_ <a id="17bcc29e" href="#17bcc29e">toPtrUnsafe</a>

```cpp
inline T toPtrUnsafe () const 
```

Unsafe cast this object into any pointer of type T. 

**Exceptions:**
* _[TypeException](ssq_TypeException.html):_ if this object is not an instance 


### _function_ <a id="54c8dbb1" href="#54c8dbb1">operator=</a>

```cpp
Object & operator= (
    const Object & other
) 
```

Copy assingment operator. 

### _function_ <a id="6272d602" href="#6272d602">operator=</a>

```cpp
Object & operator= (
    Object && other
) 
```

Move assingment operator. 



