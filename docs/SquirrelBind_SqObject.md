SqObject
===================================

Raw Squirrel object. 

**Implemented by:** [SquirrelBind::SqArray](SquirrelBind_SqArray.html), [SquirrelBind::SqClass](SquirrelBind_SqClass.html), [SquirrelBind::SqEnum](SquirrelBind_SqEnum.html), [SquirrelBind::SqFunction](SquirrelBind_SqFunction.html), [SquirrelBind::SqInstance](SquirrelBind_SqInstance.html), [SquirrelBind::SqScript](SquirrelBind_SqScript.html), [SquirrelBind::SqTable](SquirrelBind_SqTable.html), 

The documentation for this class was generated from: `include/squirrelbind/object.hpp`



## Protected Attributes

| Type | Name |
| -------: | :------- |
|  HSQUIRRELVM | [vm](#ad2f8037) |
|  HSQOBJECT | [obj](#822514a2) |
|  bool | [weak](#d431d6b9) |


## Public Functions

| Type | Name |
| -------: | :------- |
|   | [SqObject](#25339110) ()  _Creates an empty object with null VM._ |
|   | [SqObject](#ca9bc53d) (HSQUIRRELVM _vm_)  _Creates an empty object._ |
|  virtual  | [~SqObject](#8e9dfbcc) ()  |
|  void | [swap](#23b1ff90) ([SqObject](SquirrelBind_SqObject.html) & _other_)  _Swaps two objects._ |
|   | [SqObject](#4f3ec0b9) (const [SqObject](SquirrelBind_SqObject.html) & _other_)  _Copy constructor to copy the object reference._ |
|   | [SqObject](#f5ce8356) ([SqObject](SquirrelBind_SqObject.html) && _other_)  _Move constructor._ |
|  bool | [isEmpty](#98ffb556) () const  _Checks if the object is empty._ |
|  const HSQOBJECT & | [getRaw](#275f2bf5) () const  _Returns raw Squirrel object reference._ |
|  HSQOBJECT & | [getRaw](#6cb6e354) ()  _Returns raw Squirrel object reference._ |
|  [SqObject](SquirrelBind_SqObject.html) | [find](#a1195540) (const char * _name_) const  _Finds object within this object._ |
|  SqType | [getType](#4862ad08) () const  _Returns the type of the object._ |
|  const char * | [getTypeStr](#195b70ec) () const  _Returns the type of the object in string format._ |
|  const HSQUIRRELVM & | [getHandle](#d9628eef) () const  _Returns the Squirrel virtual machine handle associated with this instance._ |
|  size_t | [getTypeTag](#52d6649d) () const  _Returns the typetag associated with this object._ |
|  bool | [isNull](#300c2f7f) () const  _Returns true if the object is nullptr._ |
|  void | [reset](#acc34624) ()  _Releases the object and resets it to empty._ |
|  int32_t | [toInt](#d2994b5e) () const  _Returns the integer value of this object._ |
|  float | [toFloat](#24919b29) () const  _Returns the float value of this object._ |
|  std::string | [toString](#3f158a8a) () const  _Returns the string value of this object._ |
|  bool | [toBool](#981e4167) () const  _Returns the boolean value of this object._ |
|  [SqFunction](SquirrelBind_SqFunction.html) | [toFunction](#dfbf57b1) () const  _Returns the SqFunction value of this object._ |
|  [SqInstance](SquirrelBind_SqInstance.html) | [toInstance](#c8b67665) () const  _Returns the SqInstance value of this object._ |
|  [SqClass](SquirrelBind_SqClass.html) | [toClass](#30f718d8) () const  _Returns the SqClass value of this object._ |
|  [SqTable](SquirrelBind_SqTable.html) | [toTable](#1eef510c) () const  _Returns the SqTable value of this object._ |
|  [SqArray](SquirrelBind_SqArray.html) | [toArray](#dab33dbb) () const  _Returns the SqArray value of this object._ |
|  T | [to](#7848c8fa) () const  _Returns an arbitary value of this object._ |
|  T | [toPtrUnsafe](#6c6f6ffd) () const  _Unsafe cast this object into any pointer of type T._ |
|  [SqObject](SquirrelBind_SqObject.html) & | [operator=](#3fa644e5) (const [SqObject](SquirrelBind_SqObject.html) & _other_)  _Copy assingment operator._ |
|  [SqObject](SquirrelBind_SqObject.html) & | [operator=](#c9a0f0a8) ([SqObject](SquirrelBind_SqObject.html) && _other_)  _Move assingment operator._ |


## Protected Attributes Documentation

### _variable_ <a id="ad2f8037" href="#ad2f8037">vm</a>

```cpp
HSQUIRRELVM vm
```



### _variable_ <a id="822514a2" href="#822514a2">obj</a>

```cpp
HSQOBJECT obj
```



### _variable_ <a id="d431d6b9" href="#d431d6b9">weak</a>

```cpp
bool weak
```





## Public Functions Documentation

### _function_ <a id="25339110" href="#25339110">SqObject</a>

```cpp
 SqObject () 
```

Creates an empty object with null VM. 

**note:** This object won't be usable 


### _function_ <a id="ca9bc53d" href="#ca9bc53d">SqObject</a>

```cpp
 SqObject (
    HSQUIRRELVM vm
) 
```

Creates an empty object. 

### _function_ <a id="8e9dfbcc" href="#8e9dfbcc">~SqObject</a>

```cpp
virtual  ~SqObject () 
```



### _function_ <a id="23b1ff90" href="#23b1ff90">swap</a>

```cpp
void swap (
    SqObject & other
) 
```

Swaps two objects. 

### _function_ <a id="4f3ec0b9" href="#4f3ec0b9">SqObject</a>

```cpp
 SqObject (
    const SqObject & other
) 
```

Copy constructor to copy the object reference. 

### _function_ <a id="f5ce8356" href="#f5ce8356">SqObject</a>

```cpp
 SqObject (
    SqObject && other
) 
```

Move constructor. 

### _function_ <a id="98ffb556" href="#98ffb556">isEmpty</a>

```cpp
bool isEmpty () const 
```

Checks if the object is empty. 

### _function_ <a id="275f2bf5" href="#275f2bf5">getRaw</a>

```cpp
const HSQOBJECT & getRaw () const 
```

Returns raw Squirrel object reference. 

### _function_ <a id="6cb6e354" href="#6cb6e354">getRaw</a>

```cpp
HSQOBJECT & getRaw () 
```

Returns raw Squirrel object reference. 

### _function_ <a id="a1195540" href="#a1195540">find</a>

```cpp
SqObject find (
    const char * name
) const 
```

Finds object within this object. 

### _function_ <a id="4862ad08" href="#4862ad08">getType</a>

```cpp
SqType getType () const 
```

Returns the type of the object. 

### _function_ <a id="195b70ec" href="#195b70ec">getTypeStr</a>

```cpp
const char * getTypeStr () const 
```

Returns the type of the object in string format. 

### _function_ <a id="d9628eef" href="#d9628eef">getHandle</a>

```cpp
const HSQUIRRELVM & getHandle () const 
```

Returns the Squirrel virtual machine handle associated with this instance. 

### _function_ <a id="52d6649d" href="#52d6649d">getTypeTag</a>

```cpp
size_t getTypeTag () const 
```

Returns the typetag associated with this object. 

**note:** The typetag is equal to hash value of typeid(T) 


### _function_ <a id="300c2f7f" href="#300c2f7f">isNull</a>

```cpp
bool isNull () const 
```

Returns true if the object is nullptr. 

### _function_ <a id="acc34624" href="#acc34624">reset</a>

```cpp
void reset () 
```

Releases the object and resets it to empty. 

### _function_ <a id="d2994b5e" href="#d2994b5e">toInt</a>

```cpp
int32_t toInt () const 
```

Returns the integer value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not an integer 


### _function_ <a id="24919b29" href="#24919b29">toFloat</a>

```cpp
float toFloat () const 
```

Returns the float value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a float 


### _function_ <a id="3f158a8a" href="#3f158a8a">toString</a>

```cpp
std::string toString () const 
```

Returns the string value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a sring 


### _function_ <a id="981e4167" href="#981e4167">toBool</a>

```cpp
bool toBool () const 
```

Returns the boolean value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a boolean 


### _function_ <a id="dfbf57b1" href="#dfbf57b1">toFunction</a>

```cpp
SqFunction toFunction () const 
```

Returns the [SqFunction](SquirrelBind_SqFunction.html) value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a function 


### _function_ <a id="c8b67665" href="#c8b67665">toInstance</a>

```cpp
SqInstance toInstance () const 
```

Returns the [SqInstance](SquirrelBind_SqInstance.html) value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not an instance 


### _function_ <a id="30f718d8" href="#30f718d8">toClass</a>

```cpp
SqClass toClass () const 
```

Returns the [SqClass](SquirrelBind_SqClass.html) value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a class 


### _function_ <a id="1eef510c" href="#1eef510c">toTable</a>

```cpp
SqTable toTable () const 
```

Returns the [SqTable](SquirrelBind_SqTable.html) value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not a table 


### _function_ <a id="dab33dbb" href="#dab33dbb">toArray</a>

```cpp
SqArray toArray () const 
```

Returns the [SqArray](SquirrelBind_SqArray.html) value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not an array 


### _function_ <a id="7848c8fa" href="#7848c8fa">to</a>

```cpp
T to () const 
```

Returns an arbitary value of this object. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not an type of T 


### _function_ <a id="6c6f6ffd" href="#6c6f6ffd">toPtrUnsafe</a>

```cpp
inline T toPtrUnsafe () const 
```

Unsafe cast this object into any pointer of type T. 

**Exceptions:**
* _[SqTypeException](SquirrelBind_SqTypeException.html):_ if this object is not an instance 


### _function_ <a id="3fa644e5" href="#3fa644e5">operator=</a>

```cpp
SqObject & operator= (
    const SqObject & other
) 
```

Copy assingment operator. 

### _function_ <a id="c9a0f0a8" href="#c9a0f0a8">operator=</a>

```cpp
SqObject & operator= (
    SqObject && other
) 
```

Move assingment operator. 



