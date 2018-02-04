#include <simplesquirrel/simplesquirrel.hpp>
#include <iomanip>
#include <algorithm>

enum WeekDay {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
};

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::MATH);

    try {
        // You can cast the enums to integers if you want.
        // If you use raw objects (no casting) the the enum values will be converted
        // to userdata. With userdata, you won't be able to use '==' operator in Squirrel.
        ssq::Enum enm = vm.addEnum("WeekDay");
        enm.addSlot("MONDAY", (int)WeekDay::MONDAY);
        enm.addSlot("TUESDAY", (int)WeekDay::TUESDAY);
        enm.addSlot("WEDNESDAY", (int)WeekDay::WEDNESDAY);
        enm.addSlot("THURSDAY", (int)WeekDay::THURSDAY);
        enm.addSlot("FRIDAY", (int)WeekDay::FRIDAY);
        enm.addSlot("SATURDAY", (int)WeekDay::SATURDAY);
        enm.addSlot("SUNDAY", (int)WeekDay::SUNDAY);

        // Compile script and run it
        ssq::Script script = vm.compileFile("example_enums.nut");

        // Run the script
        vm.run(script);

        ssq::Function getThirdDay = vm.findFunc("getThirdDay");
        ssq::Object firstDay = vm.callFunc(getThirdDay, vm);

        // Prints true
        std::cout << "getThirdDay " << (firstDay.toInt() == (int)WeekDay::WEDNESDAY ? "true" : "false") << std::endl;

        ssq::Function isWednesday = vm.findFunc("isWednesday");
        bool ret = vm.callFunc(isWednesday, vm, (int)WeekDay::WEDNESDAY).toBool();

        // Prints true
        std::cout << "isWednesday: " << (ret ? "true" : "false") << std::endl;

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
