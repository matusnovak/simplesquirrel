#define SQUIRREL_STATIC
#include <squirrelbind/squirrelbind.hpp>

using namespace SquirrelBind;


int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    SqVM vm(1024, SqLibs::STRING | SqLibs::MATH);

    try {
        // Compile script and run it
        SqScript script = vm.compileFile("example_arrays.nut");

        // Run the script
        vm.run(script);

        SqFunction funcSet = vm.findFunc("set");
        SqFunction funcGet = vm.findFunc("get");
        SqFunction funcPrintContents = vm.findFunc("printContents");

        // Get the table using function get
        SqArray arr = vm.callFunc(funcGet, vm).toArray();

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

    return 0;
}
