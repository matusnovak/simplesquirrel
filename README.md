# SquirrelBind
Yet another simple binding in C++11 for [Squirrel scripting language](http://www.squirrel-lang.org/)

API Documentation can be found here: <https://matusnovak.github.io/squirrel_bind/docs/SquirrelBind.html>

## Features

* MIT licensed
* 32 and 64 bit support
* C++11
* Supports multiple virtual machines
* Supports lambdas
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

## Install Squirrel

First, download and build Squirrel library using cmake. Use the command line instructions below.
By default the `CMAKE_INSTALL_PREFIX` will point to `C:/Program Files/squirrel` on Windows or
`/home/username/squirrel` on Linux.

**Linux:**

```bash
git clone https://github.com/albertodemichelis/squirrel.git
cd squirrel
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_C_FLAGS="-fPIC"

# Build and install into the install prefix folder
sudo make install

# Copy header files (This is missing from cmake instructions in squirrel library)
sudo cp -rfv ../include* /usr/local/include
find . -name "/usr/local/include/sq*.h" -exec chmod 644 {} \;
```

**Windows:**

```bash
git clone https://github.com/albertodemichelis/squirrel.git
cd squirrel
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=C:/whatever/folder/you/want
make install
```

## Install Squirrel Bind

To compile and install Squirrel Bind use the command line instructions below. I highly recommend
using the GUI tool `cmake-gui` and set it up from there using your favourite IDE as generator.

**Linux:**

```bash
git clone https://github.com/matusnovak/squirrel_bind.git
cd squirrel_bind
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
sudo make install
```

**Windows:**

```bash
git clone https://github.com/matusnovak/squirrel_bind.git
cd squirrel_bind
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017" -DCMAKE_INSTALL_PREFIX=C:/whatever/folder/you/want
```

If you get errors that `SQSTDLIB_LIBRARIES` and `SQUIRREL_LIBRARIES` was not found, you need to
point it to the right direction when running cmake command. Either **open GUI version of cmake** and set
the `SQSTDLIB_LIBRARIES` and `SQUIRREL_LIBRARIES` so it points to the correct `squirrel.lib` and 
`sqstdlib.lib`. Don't forget to set `INCLUDE_DIRECTORIES` to Squirrel include directory! (the directory
that contains `squirrel.h` file)

If you prefer command line, use the following command:

```
cmake .. -G "Visual Studio 15 2017" \
    -DCMAKE_INSTALL_PREFIX=C:/whatever/folder/you/want \
    -DINCLUDE_DIRECTORIES=C:/Users/username/Documents/squirrel/include \
    -DSQUIRREL_LIBRARIES=C:/Users/username/Documents/squirrel/build/squirrel/MinSizeRel/squirrel_static.lib \
    -DSQSTDLIB_LIBRARIES=C:/Users/username/Documents/squirrel/build/sqstdlib/MinSizeRel/sqstdlib_static.lib \
```

## Create Squirrel VM

Creating VM is done via its constructor. The VM will be destroyed
once it falls out of scope. Or, if you have created it via `new` operator,
it will be destroyed, and all of its resources, once you call `delete`.

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    // Create and register Squirrel std libraries
    // More here: http://squirrel-lang.org/squirreldoc/stdlib/index.html
    SqVM vm(1024, SqLibs::STRING | SqLibs::IO | SqLibs::MATH);

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
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    SqVM vm(1024, SqLibs::ALL);

    SqScript scriptA = vm.compileSource(/* raw char array here */);
    SqScript scriptB = vm.compileFile(/* path to source file */);

    return 0;
}
```

## Squirrel object manipulation

All Squirrel objects are dynamic and they can hold any value, no static typing. Since C++
is statically typed, we need to check the type returned. You can do so by calling `SqObject::getType()` 
which returns `SqType` enum. The object can be: instance, class, function (closure), integer, float,
string, boolean, userdata, userpointer, array, table, and null. You can convert the raw object to
any type using `SqObject::toXyz();` such as: `toInt()`, `toFloat()`, `toString()`, `toBool()`, 
`toTable()`, `toArray()`, `toClass()`, `toInstance()`, and `toFunction()`. To check if the object is
empty use `isEmpty()` method and to check if the object is null use `isNull()`. Note that object being
null is not the same as object being empty! However, it is impossible for Squirrel to use empty objects
so you will most likely never use `isEmpty()`, only `isNull()`.

**Always use std::string when calling Squirrel function or when casting the result of function
call from SqObject to string. Do not use const char\* as those two types are different.**

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
| SqObject | anything | You can't convet SqObject to SqObject :D |
| SqFunction | closure/nativeclosure | `toFunction()` |
| SqClass | class | `toClass()` |
| SqInstance | instance | `toInstance()` |
| SqTable | table | `toTable()` |
| SqArray | array | `toArray()` |
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
**Getting double from SqObject on 32-bit compiler will result in loss of information!**

Also, unsigned types are converted into signed types. For example, passing 0xFFFFFFFF (the max value of 
unsigned long) will be represented in squirrel as -1. However, when you try to get the -1 from Squirrel
back to C++ and you convert the function call result (from SqObject) to `to<unsigned long>()` it 
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
SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");
SqObject result = vm.call(mySquirrelFunc, vm, ...);
Foo copy = result.to<Foo>();
```

However the following will return pointer to the instance, no copy performed:

```cpp
SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");
SqObject result = vm.call(mySquirrelFunc, vm, ...);
Foo* copy = result.to<Foo*>();
```

Here is an example how to handle return value from function call:

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    SqVM vm(1024, SqLibs::ALL);
    SqScript script = vm.compileSource(...);
    vm.run(script);

    SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");

    // Get the result of the function call
    SqObject result = vm.call(mySquirrelFunc, vm, 10, true, std::string("Hello World"));

    // Check if the object is empty
    result.isEmpty(); // Returns true or false depending on what has been returned

    // Check if object is null
    result.isNull(); // True or false depending on what has been returned

    // Check if object is an integer
    if (result.getType() == SqType::INTEGER) {
        // Object is integer, we can convert it
        int myInt = result.toInt();
        // or:
        long myInt2 = result.to<long>();
    }

    // Converting to something else than the object contains
    // results in SqTypeException thrown
    try {
        std::string str = result.toString();
    } catch (SqTypeException& e){
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
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

std::string myCppFunc(int a, int b){
    return std::to_string(a + b);
}

int main(){
    SqVM vm(1024, SqLibs::ALL);

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
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

std::string myCppFunc(int a, int b){
    return std::to_string(a + b);
}

int main(){
    SqVM vm(1024, SqLibs::ALL);

    static const std::string source = 
    "function mySquirrelFunc(a, b){\n"
    "    return a + b;\n"
    "}\n";

    SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

    // Now the mySquirrelFunc is ready for us, let's
    // find it from VM
    SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");

    // We can get the number of parameters required via:
    // mySquirrelFunc.getNumOfParams()

    // Call the function
    // The second parameter is the "this" pointer!
    // If the function was part of class (member function) then
    // the second parameter needs to be an instance object.
    // Since the mySquirrelFunc is global, we will pass the
    // root table -> the VM instance.
    SqObject result = vm.call(mySquirrelFunc, vm, 10, 20);

    // We have the result and we can check its type via:
    // result.getType() which returns SqType enum
    // Or get the type as const char*
    // result.getTypeStr()

    // Convert the result to integer
    int myInt = result.toInt();

    std::cout << "mySquirrelFunc returned: " << myInt << std::endl;

    return 0;
}
```

## Bind C++ class

Binding of classes is done via `SqVM::addClass(...)`. You have to expose your class to VM. Otherwise 
any of your classes will be passed around in squirrel as user data and not as an instance. 

You don't need to expose your class if you do not wish to use it in Squirrel as an instance. Any
classes or any types that are not integers, strings, booleans, will be automatically converted into 
Squirrel user data. User data can be moved around, pushed to Squirrel or returned from. User data 
cannot be manipulated, has no methods, and is created via copy. For example, you won't be able 
to use `std::unique_ptr` as userdata because it does not allow a copy.

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

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

    static void expose(SqVM& vm) {
        SqClass cls = vm.addClass("Foo", SqClass::Ctor<Foo(std::string)>());

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
    SqVM vm(1024, SqLibs::ALL);

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

    SqScript script = vm.compileSource(source.c_str());
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

    static SqClass expose(SqVM& vm);
};
```

One gets to be used when you have constant reference (or pointer) and the second one
when you have non-constant reference. Binding these methods to Squirrel class can
cause compiler ambiguity error. You can solve the problem by explicitly defining the
template when binding those functions, for example:

```
static void Foo::expose(SqVM& vm) {
    SqClass cls = vm.addClass("Foo", SqClass::Ctor<Foo(.....)>());

    // Explicitly define return type to "const std::string&" in order
    // for the compiler to know which overloaded method to use.
    cls.addFunc<const std::string&>("getMessage", &Foo::getMessage);
}
```

## Find Squirrel class and create instance

Finding classes and creating instances is easy as the following code below. 
It is self explanatory.

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    SqVM vm(1024, SqLibs::ALL);

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

    SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

    // Find class
    SqClass cls = vm.findClass("Foo");

    // Create instance of it.
    // The life of the object is determined by Squirrel.
    // You don't need to release the object yourself.
    SqInstance clsInstance = vm.newInstance(cls, std::string("Hello World!"));

    // Find class methods
    SqFunction funcSetMsg = cls.findFunc("setMsg");
    SqFunction funcGetMsg = cls.findFunc("getMsg");

    // Call method, set new message
    vm.callFunc(funcSetMsg, clsInstance, "Banana");

    // Get the message back
    std::string result = vm.callFunc(funcGetMsg, clsInstance).toString();

    std::cout << "Foo now contains: " << result << std::endl;

    return 0;
}
```

## Manipulate Squirrel array

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    SqVM vm(1024, SqLibs::ALL);
    SqScript script = vm.compileSource(...);
    vm.run(script);

    // Create new array
    SqArray arr = vm.newArray();

    // To get the size of the array use: arr.size() 

    // Push at the back 
    // You can push any object you want. No limits.
    arr.push(std::string("Hello World"));
    arr.push(true);
    arr.push(123);

    // You can retrieve individual indexes via:
    std::string first = arr.get<std::string>(0);

    // If you want raw object (SqObject) do it as:
    SqObject firstRaw = arr.get<SqObject>(0);

    // Use pop to release last element
    int arr.popAndGet<int>();

    // If you do not want to get the last element while removing it
    // use the following:
    arr.pop(); // Returns nothing

    // Then, simply pass it into any squirrel function as any other value
    SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");
    vm.call(mySquirrelFunc, vm, arr);

    // If you want to retrieve array from squirrel function,
    // you can do it simply by converting the return value to array:
    SqArray result = vm.call(..., vm).toArray();

    return 0;
}
```

## Manipulate Squirrel table

```cpp
#include <squirrelbind/squirrelbind.hpp>
using namespace SquirrelBind;

int main(){
    SqVM vm(1024, SqLibs::ALL);
    SqScript script = vm.compileSource(...);
    vm.run(script);

    // Create new table
    SqTable table = vm.newTable();

    // To get the size of the table use: table.size() 

    // Add new indexes via key-value pairs
    // You can add any object you want. No limits.
    table.set("myString", std::string("Hello World"));
    table.set("myBool", true);
    table.set("myInt", 123);

    // You can retrieve individual indexes via:
    std::string first = table.get<std::string>("myString");

    // If you want raw object (SqObject) do it as:
    SqObject firstRaw = table.get<SqObject>("myString");

    // Then, simply pass it into any squirrel function as any other value
    SqFunction mySquirrelFunc = vm.findFunc("mySquirrelFunc");
    vm.call(mySquirrelFunc, vm, table);

    // If you want to retrieve table from squirrel function,
    // you can do it simply by converting the return value to table:
    SqTable result = vm.call(..., vm).toTable();

    return 0;
}
```