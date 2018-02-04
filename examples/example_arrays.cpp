#include <simplesquirrel/simplesquirrel.hpp>

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::MATH);

    try {
        // Compile script and run it
        ssq::Script script = vm.compileFile("example_arrays.nut");

        // Run the script
        vm.run(script);

        ssq::Function funcSet = vm.findFunc("set");
        ssq::Function funcGet = vm.findFunc("get");
        ssq::Function funcPrintContents = vm.findFunc("printContents");

        // Get the table using function get
        ssq::Array arr = vm.callFunc(funcGet, vm).toArray();

        // Get the size of the array
        std::cout << "We got array of length: " << arr.size() << std::endl;

        std::cout << "first: " << arr.get<int>(0) << std::endl;
        std::cout << "second: " << arr.get<std::string>(1) << std::endl;

        // Add new slot into the table
        arr.set(0, 42);
        arr.set(1, std::string("Hello World"));

        // Push value at the end
        arr.push(std::string("Banana"));

        // Return the table back
        vm.callFunc(funcSet, vm, arr);

        // Call printContents
        vm.callFunc(funcPrintContents, vm);

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

    return 0;
}
