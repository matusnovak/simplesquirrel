#include <simplesquirrel/simplesquirrel.hpp>
#include <iomanip>

// Some random global function
std::string appendString(const std::string& str) {
    return "Hello " + str + "!";
}

// Another random global function
std::string getLocalTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);
    return str;
}

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::MATH);

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
        ssq::Script script = vm.compileFile("example_functions.nut");
        vm.run(script);

        // Once the script is compiled, its functions are exposed to us
        // Find the foo and bar functions
        ssq::Function fooFunc = vm.findFunc("foo");
        ssq::Function barFunc = vm.findFunc("bar");

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
        ssq::Object ret = vm.callFunc(barFunc, vm, 10, 5, 3);
        std::cout << "Squirrel foo returned: " << ret.toInt() << std::endl;

    } catch (ssq::CompileException& e) {
        std::cerr << "Failed to run file: " << e.what() << std::endl;
        return -1;
    } catch (ssq::TypeException& e) {
        std::cerr << "Something went wrong passing objects: " << e.what() << std::endl;
        return -1;
    } catch (ssq::RuntimeException& e) {
        std::cerr << "Something went wrong during execution: " << e.what() << std::endl;
        return -1;
    } catch (ssq::NotFoundException& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}