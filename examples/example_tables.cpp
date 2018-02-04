#include <simplesquirrel/simplesquirrel.hpp>

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::MATH);

    try {
        // Compile script and run it
        ssq::Script script = vm.compileFile("example_tables.nut");

        // Run the script
        vm.run(script);

        ssq::Function funcSet = vm.findFunc("set");
        ssq::Function funcGet = vm.findFunc("get");
        ssq::Function funcPrintContents = vm.findFunc("printContents");

        // Get the table using function get
        ssq::Table table = vm.callFunc(funcGet, vm).toTable();

        // Add new slot into the table
        table.set("banana", std::string("potasium"));

        // Return the table back
        vm.callFunc(funcSet, vm, table);

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
