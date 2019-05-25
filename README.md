# SimpleSquirrel

|         | CircleCI | TravisCI | AppVeyor |
|---------|----------|----------|----------|
| master  | [![CircleCI](https://circleci.com/gh/matusnovak/simplesquirrel/tree/master.svg?style=svg)](https://circleci.com/gh/matusnovak/simplesquirrel/tree/master) | [![Build Status](https://travis-ci.org/matusnovak/simplesquirrel.svg?branch=master)](https://travis-ci.org/matusnovak/simplesquirrel) | [![Build status](https://ci.appveyor.com/api/projects/status/hk9m6f62csx8llvy/branch/master?svg=true)](https://ci.appveyor.com/project/matusnovak/simplesquirrel/branch/devel) |
| devel  | [![CircleCI](https://circleci.com/gh/matusnovak/simplesquirrel/tree/devel.svg?style=svg)](https://circleci.com/gh/matusnovak/simplesquirrel/tree/devel) | [![Build Status](https://travis-ci.org/matusnovak/simplesquirrel.svg?branch=devel)](https://travis-ci.org/matusnovak/simplesquirrel) | [![Build status](https://ci.appveyor.com/api/projects/status/hk9m6f62csx8llvy/branch/devel?svg=true)](https://ci.appveyor.com/project/matusnovak/simplesquirrel/branch/devel) |

*The following compilers are tested in the CI above: Visual Studio 2015, Visual Studio 2015 Win64, Visual Studio 2017, Visual Studio 2017 Win64, MinGW-w64 i686, MinGW-w64 x86_64, Linux GCC 5.5.0, Linux GCC 6.4.0, Linux GCC 7.3.0, Linux GCC 8.2.0, and OSX Clang 3.7*

Yet another simple binding in C++11 for [Squirrel scripting language](http://www.squirrel-lang.org/)

API Documentation can be found here: <https://matusnovak.github.io/simplesquirrel/doxygen/group__simplesquirrel.html>

## Features

* MIT licensed
* 32 and 64 bit support
* C++11
* Supports multiple virtual machines
* Supports lambdas
* Works on Windows (Visual Studio 2015 or newer) (or MinGW 4.9 or newer)
* Works on Linux (GCC 4.9 or newer)
* Works on OSX (Clang 3.7 or newer)
* Very easy object manipulation
* Supports binding of C++ classes to multiple instances of Squirrel VM
* Strict type checking on C++ side
* wchar\_t support, a.k.a Squirrel Unicode (experimental!) - I highly recommend using UTF8 as chars and not UNICODE as wchar\_t
* Works with Visual Studio 2015, Visual Studio 2017, MinGW-w64, Linux GCC, and OSX Clang 
* Allows the following:
  * Binding C++ function and calling it from Squirrel
  * Looking up Squirrel function and calling it from C++
  * Looking up Squirrel classes and creating instances on C++ side
  * Binding C++ classes including methods and member variables
  * Passing any C++ pointer as either instance or userpointer (depends if your C++ has been exposed to VM)
  * Creating enumerations
  * Creating and passing tables
  * Creating and passing arrays
* **The following is not yet implemented:**
  * Method overloading
  * Derivate Squirrel class
  * **Thread safety**

## Installation

You can [use one of the automatically built releases from GitHub Releases](https://github.com/matusnovak/simplesquirrel/releases) or compile it manually by following the [Install Guide](./INSTALL.md)

## Create Squirrel VM

Creating VM is done via its constructor. The VM will be destroyed
once it falls out of scope. Or, if you have created it via `new` operator,
it will be destroyed, and all of its resources, once you call `delete`.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    // Create and register Squirrel std libraries
    // More here: http://squirrel-lang.org/squirreldoc/stdlib/index.html
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::IO | ssq::Libs::MATH);

    // Available flags:
    // NONE
    // IO
    // BLOB
    // MATH
    // SYSTEM
    // STRING
    // ALL

    return 0;
}
```

## Compile script

Compiling script can be done via raw source `const char*` or via path to source
file.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);

    ssq::Script scriptA = vm.compileSource(/* raw char array here */);
    ssq::Script scriptB = vm.compileFile(/* path to source file */);

    return 0;
}
```

## Squirrel object manipulation

All Squirrel objects are dynamic and they can hold any value, no static typing. Since C++
is statically typed, we need to check the type returned. You can do so by calling `ssq::Object::getType()` 
which returns `ssq::Type` enum. The object can be: instance, class, function (closure), integer, float,
string, boolean, userdata, userpointer, array, table, and null. You can convert the raw object to
any type using `ssq::Object::toXyz();` such as: `toInt()`, `toFloat()`, `toString()`, `toBool()`, 
`toTable()`, `toArray()`, `toClass()`, `toInstance()`, and `toFunction()`. To check if the object is
empty use `isEmpty()` method and to check if the object is null use `isNull()`. Note that object being
null is not the same as object being empty! However, it is impossible for Squirrel to use empty objects
so you will most likely never use `isEmpty()`, only `isNull()`.

**Always use std::string when calling Squirrel function or when casting the result of function
call from ssq::Object to string. Do not use const char\* as those two types are different.**

Here are the C++ types and their Squirrel equivalents:

| C++ | Squirrel | Method to convert back |
| --- | -------- | ---------------------- |
| signed char | integer | `toInt()` |
| unsigned char | integer | `toInt()` |
| char | integer | `toInt()` |
| short | integer | `toInt()` |
| unsigned short | integer | `toInt()` |
| int | integer | `toInt()` |
| unsigned int | integer | `toInt()` |
| long | integer | `toInt()` |
| unsigned long | integer | `toInt()` |
| long long (64-bit) | integer | `toInt()` |
| long long (32-bit) | userdata | `to<long long>()` |
| unsigned long long (64-bit) | integer | `toInt()` |
| unsigned long long (64-bit) | userdata | `to<unsigned long long>()` |
| std::string | string | `toString()` |
| float | float | `toFloat()` |
| double | float | `toFloat()` |
| const char* | userpointer | `to<const char*>()` |
| const char[N] | N/A | compiler error |
| char* | userpointer | `to<char*>()` |
| ssq::Object | anything | You can't convet ssq::Object to ssq::Object :D |
| ssq::Function | closure/nativeclosure | `toFunction()` |
| ssq::Class | class | `toClass()` |
| ssq::Instance | instance | `toInstance()` |
| ssq::Table | table | `toTable()` |
| ssq::Array | array | `toArray()` |
| void* | userpointer | `to<void*>()` |
| any pointer to class **not added to VM via addClass** | userpointer | `to<Class_Type*>()` |
| any reference (or copy) to class **not added to VM via addClass** | userdata | `to<Class_Type>()` |
| pointer to class | instance | `to<Class_Type*>()` |
| reference or copy to class | instance | `to<Class_Type>()` |

Some types are divided into 32-bit and 64-bit groups. This is due to the fact how Squirrel handles integers.
On 64-bit compiler, the integer is stored as `long long`, on 32-bit compiler, it is stored as `long`. Therefore
in order to pass `long long` into squirrel on 32-bit compiler, it will be converted into userdata. We can't 
store 64-bit integer in 32-integer. Conversion to userdata is done to preserve the information. On 64-bit compiler
this is simply an integer. Floats are also stored as float on 32-bit compiler and double on 64-bit compiler.
**Getting double from ssq::Object on 32-bit compiler will result in loss of information!**

Also, unsigned types are converted into signed types. For example, passing 0xFFFFFFFF (the max value of 
unsigned long) will be represented in squirrel as -1. However, when you try to get the -1 from Squirrel
back to C++ and you convert the function call result (from ssq::Object) to `to<unsigned long>()` it 
will be exactly the same as previously, the 0xFFFFFFFF. Information is always preserved, nothing is lost.
Therefore I highly suggest to use signed integers only.

Passing instance of classes as copy (or reference) will result in copy of the class
and the life of the instance will be handled by Squirrel. Passing any instance of class as a pointer 
will **not** create a copy and Squirrel will not handle the life of the instance.

If you create a C++ method where you create new instance of class and you return it as a pointer,
(example: `return new Foo(123, 456);`), the Squirrel will be able to access the methods and manipulate the 
object. However, it will never be released unless you do it manually from C++ side (by tracking your 
allocated objects).

If you return stack allocated object (example: `return Foo(123, 456);`) the Squirrel will create
a copy of the object and the life of the given object will be also handled by Squirrel.

For example, the following code will return copy of the instance inside of Squirrel:

```cpp
ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");
ssq::Object result = vm.call(mySquirrelFunc, vm, ...);
Foo copy = result.to<Foo>();
```

However the following will return pointer to the instance, no copy performed:

```cpp
ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");
ssq::Object result = vm.call(mySquirrelFunc, vm, ...);
Foo* copy = result.to<Foo*>();
```

Here is an example how to handle return value from function call:

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);
    ssq::Script script = vm.compileSource(...);
    vm.run(script);

    ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");

    // Get the result of the function call
    ssq::Object result = vm.call(mySquirrelFunc, vm, 10, true, std::string("Hello World"));

    // Check if the object is empty
    result.isEmpty(); // Returns true or false depending on what has been returned

    // Check if object is null
    result.isNull(); // True or false depending on what has been returned

    // Check if object is an integer
    if (result.getType() == ssq::Type::INTEGER) {
        // Object is integer, we can convert it
        int myInt = result.toInt();
        // or:
        long myInt2 = result.to<long>();
    }

    // Converting to something else than the object contains
    // results in ssq::TypeException thrown
    try {
        std::string str = result.toString();
    } catch (ssq::TypeException& e){
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
```

## Binding C++ function

Binding C++ functions is simple as described below. The functions will be added 
into the root table and can be used across all scripts compiled within the VM.
The function arguments are deduced at compile time. No std::tuple, no std::any, 
just good old metaprogramming.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

std::string myCppFunc(int a, int b){
    return std::to_string(a + b);
}

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);

    // Bind global function
    vm.addFunc("myCppFunc", myCppFunc);

    // Bind global function via std::function
    std::function<std::string(int, int)> func = std::bind(
        myCppFunc, 
        std::placeholders::_1, 
        std::placeholders::_2
    );
    vm.addFunc("myCppFunc2", func);

    // Bind via lambda
    vm.addFunc("myCppFuncLambda, [](int a, int b) -> std::string {
        return std::to_string(a + b);
    });

    return 0;
}
```

## Call Squirrel global function

First, you need to find the function you are looking for. This won't be done unless you
first compile **and run** the script. Squirrel is a dynamic language, therefore the functions
are evaulated at run time.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

std::string myCppFunc(int a, int b){
    return std::to_string(a + b);
}

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);

    static const std::string source = 
    "function mySquirrelFunc(a, b){\n"
    "    return a + b;\n"
    "}\n";

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    // Now the mySquirrelFunc is ready for us, let's
    // find it from VM
    ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");

    // We can get the number of parameters required via:
    // mySquirrelFunc.getNumOfParams()

    // Call the function
    // The second parameter is the "this" pointer!
    // If the function was part of class (member function) then
    // the second parameter needs to be an instance object.
    // Since the mySquirrelFunc is global, we will pass the
    // root table -> the VM instance.
    ssq::Object result = vm.call(mySquirrelFunc, vm, 10, 20);

    // We have the result and we can check its type via:
    // result.getType() which returns ssq::Type enum
    // Or get the type as const char*
    // result.getTypeStr()

    // Convert the result to integer
    int myInt = result.toInt();

    std::cout << "mySquirrelFunc returned: " << myInt << std::endl;

    return 0;
}
```

## Bind C++ class

Binding of classes is done via `ssq::VM::addClass(...)`. You have to expose your class to VM. Otherwise 
any of your classes will be passed around in squirrel as user data and not as an instance. 

You don't need to expose your class if you do not wish to use it in Squirrel as an instance. Any
classes or any types that are not integers, strings, booleans, will be automatically converted into 
Squirrel user data. User data can be moved around, pushed to Squirrel or returned from. User data 
cannot be manipulated, has no methods, and is created via copy. For example, you won't be able 
to use `std::unique_ptr` as userdata because it does not allow a copy.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

class Foo {
public:
    Foo(const std::string& msg):msg(msg) {
    }

    const std::string& getMsg() const {
        return msg;
    }

    void setMsg(const std::string& msg) {
        this->msg = msg;
    }

    static void expose(ssq::VM& vm) {
        ssq::Class cls = vm.addClass("Foo", ssq::Class::Ctor<Foo(std::string)>());

        // You can also use lambdas (or std::function) to define constructor
        // The function has to return pointer to the new instance
        //
        // vm.addClass("Foo", [](std::string msg) -> Foo* {
        //     return new Foo(msg);
        // });

        cls.addFunc("getMsg", &Foo::getMsg);
        cls.addFunc("setMsg", &Foo::setMsg);

        cls.addVar("msg", &Foo::msg);
    }

    // For the purpose of this example we will treat msg as public
    // so we can expose it via cls.addVar
    std::string msg;
};

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);

    static const std::string source = 
    "// Create instance\n"
    "local foo = Foo(\"Hello World\");\n"
    "\n"
    "// Call C++ methods\n"
    "print(\"Foo has message: \" + foo.getMessage());\n"
    "foo.setMessage(\"Something else\");\n"
    "\n"
    "// Access the variable directly\n"
    "print(\"Foo now has: \" + foo.msg);\n"
    "\n";

    ssq::Script script = vm.compileSource(source.c_str());
    Foo::expose(vm); // Bind the class to this VM
    vm.run(script);

    return 0;
}
```

## Class const method ambiguity

Sometimes, it is possible that your class has for example two methods:

```cpp
class Foo {
    ...
    const std::string& getMessage() const;
    std::string& getMessage();

    static ssq::Class expose(ssq::VM& vm);
};
```

One gets to be used when you have constant reference (or pointer) and the second one
when you have non-constant reference. Binding these methods to Squirrel class can
cause compiler ambiguity error. You can solve the problem by explicitly defining the
template when binding those functions, for example:

```
static void Foo::expose(ssq::VM& vm) {
    ssq::Class cls = vm.addClass("Foo", ssq::Class::Ctor<Foo(.....)>());

    // Explicitly define return type to "const std::string&" in order
    // for the compiler to know which overloaded method to use.
    cls.addFunc<const std::string&>("getMessage", &Foo::getMessage);
}
```

## Find Squirrel class and create instance

Finding classes and creating instances is easy as the following code below. 
It is self explanatory.

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);

    static const std::string source = 
    "class Foo {\n"
    "    constructor(msg) {\n"
    "        value = msg;\n"
    "    }\n"
    "\n"
    "    function setMsg(msg) {\n"
    "        msg = value;\n"
    "    }\n"
    "\n"
    "    function getMsg() {\n"
    "        return msg;\n"
    "    }\n"
    "\n"
    "    value = null;\n"
    "}\n";

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    // Find class
    ssq::Class cls = vm.findClass("Foo");

    // Create instance of it.
    // The life of the object is determined by Squirrel.
    // You don't need to release the object yourself.
    ssq::Instance clsInstance = vm.newInstance(cls, std::string("Hello World!"));

    // Find class methods
    ssq::Function funcSetMsg = cls.findFunc("setMsg");
    ssq::Function funcGetMsg = cls.findFunc("getMsg");

    // Call method, set new message
    vm.callFunc(funcSetMsg, clsInstance, "Banana");

    // Get the message back
    std::string result = vm.callFunc(funcGetMsg, clsInstance).toString();

    std::cout << "Foo now contains: " << result << std::endl;

    return 0;
}
```

## Inherit from C++ class inside of Squirrel

Please note that the inherited C++ class will not be correctly constructed if you forget to call `base.constructor(...)` inside of your derived class constructor! What happens if you do not call base constructor is undefined, most likely a memory corruption. All base class methods are accessible through `base.whatever()`. It is also possible to bind base class properties and access them through `local p = base.property;` inside of your derived class.

```cpp
class Foo {
public:
    Foo(const std::string& msg):msg(msg) {
    }

    const std::string& getMsg() const {
        return msg;
    }

    void setMsg(const std::string& msg) {
        this->msg = msg;
    }

    static void expose(ssq::VM& vm) {
        ssq::Class cls = vm.addClass("Foo", ssq::Class::Ctor<Foo(std::string)>());
        cls.addFunc("getMsg", &Foo::getMsg);
        cls.addFunc("setMsg", &Foo::setMsg);
    }
private:
    std::string msg;
};

int main() {
    ssq::VM vm(1024, ssq::Libs::ALL);

    static const std::string source = 
    "class FooDerived extends Foo {\n"
    "    constructor(msg, value) {\n"
    "        base.constructor(msg);\n"
    "        val = value;\n"
    "    }\n"
    "\n"
    "    function getValue() {\n"
    "        return val;\n"
    "    }\n"
    "\n"
    "    function setValue(value) {\n"
    "        val = value;\n"
    "    }\n"
    "\n"
    "    function doSomething() {\n"
    "        local msg = base.getMsg();\n"
    "        // Now do something wth copy of Foo's std::string\n"
    "    }\n"
    "\n"
    "    val = null;\n"
    "};\n"
    "\n"
    "local derived = FooDerived("Hello", 1234);\n";
    "derived.doSomething();\n";

    Foo::expose(vm);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    return 0;
}
```

## Manipulate Squirrel array

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);
    ssq::Script script = vm.compileSource(...);
    vm.run(script);

    // Create new array
    ssq::Array arr = vm.newArray();

    // To get the size of the array use: arr.size() 

    // Push at the back 
    // You can push any object you want. No limits.
    arr.push(std::string("Hello World"));
    arr.push(true);
    arr.push(123);

    // You can retrieve individual indexes via:
    std::string first = arr.get<std::string>(0);

    // If you want raw object (ssq::Object) do it as:
    ssq::Object firstRaw = arr.get<ssq::Object>(0);

    // Use pop to release last element
    int arr.popAndGet<int>();

    // If you do not want to get the last element while removing it
    // use the following:
    arr.pop(); // Returns nothing

    // Then, simply pass it into any squirrel function as any other value
    ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");
    vm.call(mySquirrelFunc, vm, arr);

    // If you want to retrieve array from squirrel function,
    // you can do it simply by converting the return value to array:
    ssq::Array result = vm.call(..., vm).toArray();

    return 0;
}
```

## Manipulate Squirrel table

```cpp
#include <simplesquirrel/simplesquirrel.hpp>

int main(){
    ssq::VM vm(1024, ssq::Libs::ALL);
    ssq::Script script = vm.compileSource(...);
    vm.run(script);

    // Create new table
    ssq::Table table = vm.newTable();

    // To get the size of the table use: table.size() 

    // Add new indexes via key-value pairs
    // You can add any object you want. No limits.
    table.set("myString", std::string("Hello World"));
    table.set("myBool", true);
    table.set("myInt", 123);

    // You can retrieve individual indexes via:
    std::string first = table.get<std::string>("myString");

    // If you want raw object (ssq::Object) do it as:
    ssq::Object firstRaw = table.get<ssq::Object>("myString");

    // Then, simply pass it into any squirrel function as any other value
    ssq::Function mySquirrelFunc = vm.findFunc("mySquirrelFunc");
    vm.call(mySquirrelFunc, vm, table);

    // If you want to retrieve table from squirrel function,
    // you can do it simply by converting the return value to table:
    ssq::Table result = vm.call(..., vm).toTable();

    return 0;
}
```

## Weak references and callbacks

There is a problem when you want to register a callback into C++ side. For example,
you have a GUI widget that has onClick method. But, the onClick method accepts `std::function`
and not Squirrel objects. So, you will probably try to do the following:

```cpp
// This will cause undefined behavior! Most likely SEGFAULT! See another example below...
fooClass.addFunc("setOnClickCallback", [](Foo* self, ssq::Function func, ssq::Instance inst) -> void {
    // Using ssq::Instance here is OK!
    doSomethingElse(inst);

    self.setOnClick([=](SomeEventData data) {
        // Using ssq::Instance in here nested lambda with [=] capture is bad!
        vm.callFunc(func, inst, data);
    });
});

// On program exit...
SEGFAULT
```

And then inside of Squirrel:

```
class Bar extends Foo {
    ...
}

local bar = Bar(...);
baz.setOnClickCallback(function(data){
    print("I got some data!");
}, bar);
```

Why is this bad? Because the inner lambda captures the instance, it will cause to extend the
life of Foo object inside of Squirrel. When you try to destroy your VM, it will most likely cause
to crash program. Squirrel tracks objects by reference so that when reference counter goes to zero,
the object is deleted. With the example above the ssq::Instance object will live inside of the lambda
capture, extending the life of the instance. The reference counter will always be +1. This problem
only happens when you use lambda capture inside of lambda as above. 
**How to solve this? Use weak reference!** 

```cpp
// This will NOT cause any problems
fooClass.addFunc("setOnClickCallback", [](Foo* self, ssq::Function func, SqWeakRef ref) -> void {
    self.setOnClick([=](SomeEventData data) {
        vm.callFunc(func, ref, data);
    });
});

// On program exit...
// All ok... No SEGFAULT
```

With the weak reference, the life of the instance won't be extended. The weak reference does
not increament the reference counter at all. It does not matter for how long the lambda captured
variable (the `ref` parameter) will live, it won't affect us at all. 
