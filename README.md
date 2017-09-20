# simplesq
Yet another simple binding in C++11 for Squirrel scripting language

This is a header-only binding library for [Squirrel scripting language](http://www.squirrel-lang.org/) created based on [Sqrat](http://scrat.sourceforge.net/). Why another binding language? Sqrat offers more features such as overloading, however it is not C++11 friendly. Simplesq uses [std::function](http://en.cppreference.com/w/cpp/utility/functional/function) for all of its functions and offers unlimited number of arguments, and uses less memory (no global tables to keep reference of instances). 

To include this class into your project, simply add `#include <simplesq.hpp>` into your project. Don't forget to add include path `-I/path/to/this/repo/include` into your compiler command line as well! No need to pre-compile this library as everything is inlined and put into a single header file.

**Please, see <examples.cpp> for more information about how to use this library.** Don't be afraid of the `TEST_CASE` syntax. This is used by [catch.hpp](https://github.com/philsquared/Catch) unit testing and is meant only for testing the development of this library, it has nothing to do with Squirrel language or bindings itself. 

Please note that this library is not complete. It needs more testing and more features implemented such as function overloading. This library was created due to use of `std::function` as a binding, rather than raw member function pointer which Sqrat uses, including less memory usage. 