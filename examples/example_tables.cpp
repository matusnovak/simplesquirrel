#define SQUIRREL_STATIC
#include <squirrelbind/squirrelbind.hpp>

using namespace SquirrelBind;


int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    SqVM vm(1024, SqLibs::STRING | SqLibs::MATH);

    try {
        // Compile script and run it
        SqScript script = vm.compileFile("example_tables.nut");

        // Run the script
        vm.run(script);

        SqFunction funcSet = vm.findFunc("set");
        SqFunction funcGet = vm.findFunc("get");
        SqFunction funcPrintContents = vm.findFunc("printContents");

        // Get the table using function get
        SqTable table = vm.callFunc(funcGet, vm).toTable();

        // Add new slot into the table
        table.set("banana", std::string("potasium"));

        // Return the table back
        vm.callFunc(funcSet, vm, table);

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
