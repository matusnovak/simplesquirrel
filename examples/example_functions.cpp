#define SQUIRREL_STATIC
#include <squirrelbind/squirrelbind.hpp>
#include <iomanip>

using namespace SquirrelBind;

// Some random global function
std::string appendString(const std::string& str) {
    return "Hello " + str + "!";
}

// Another random global function
std::string getLocalTime() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
}

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    SqVM vm(1024, SqLibs::STRING | SqLibs::MATH);

    try {
        // Expose the global function bia function pointer
        vm.addFunc("appendString", appendString);

        // Expose another function via std::function
        std::function<std::string()> func(getLocalTime);
        vm.addFunc("getLocalTime", func);

        // Expose function via lambda
        vm.addFunc("doStuff", [](int i, float f, bool b) -> long {
            std::cout << "Got stuff from squirrel! i: " << i << " f: " << f << " b: " << (b ? "true" : "false") << std::endl;
            return 42L;
        });

        // Compile script and run it
        SqScript script = vm.compileFile("example_functions.nut");
        vm.run(script);

        // Once the script is compiled, its functions are exposed to us
        // Find the foo and bar functions
        SqFunction fooFunc = vm.findFunc("foo");
        SqFunction barFunc = vm.findFunc("bar");

        // Get the number of parameters the functions accept
        std::cout << "Squirrel foo function accepts " << fooFunc.getNumOfParams() << " params" << std::endl;
        std::cout << "Squirrel bar function accepts " << barFunc.getNumOfParams() << " params" << std::endl;

        // Call the functions
        // The second parameter is the "this" pointer. Because the functions foo and bar
        // are global, we will pass the root table (the VM)
        // If the functions foo and bar were members of a class, we would need to use instance as
        // the second parameter.
        vm.callFunc(fooFunc, vm, std::string("Hello World!"));

        // Call the bar func and expect a return value
        SqObject ret = vm.callFunc(barFunc, vm, 10, 5, 3);
        std::cout << "Squirrel foo returned: " << ret.toInt() << std::endl;

    } catch (SqCompileException& e) {
        std::cerr << "Failed to run file: " << e.what() << std::endl;
        return -1;
    } catch (SqTypeException& e) {
        std::cerr << "Something went wrong passing objects: " << e.what() << std::endl;
        return -1;
    } catch (SqRuntimeException& e) {
        std::cerr << "Something went wrong during execution: " << e.what() << std::endl;
        return -1;
    } catch (SqNotFoundException& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}