#include <squirrelbind/squirrelbind.hpp>
#include <iomanip>
#include <algorithm>

using namespace SquirrelBind;

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
    SqVM vm(1024, SqLibs::STRING | SqLibs::MATH);

    try {
        // You can cast the enums to integers if you want.
        // If you use raw objects (no casting) the the enum values will be converted
        // to userdata. With userdata, you won't be able to use '==' operator in Squirrel.
        SqEnum enm = vm.addEnum("WeekDay");
        enm.addSlot("MONDAY", (int)WeekDay::MONDAY);
        enm.addSlot("TUESDAY", (int)WeekDay::TUESDAY);
        enm.addSlot("WEDNESDAY", (int)WeekDay::WEDNESDAY);
        enm.addSlot("THURSDAY", (int)WeekDay::THURSDAY);
        enm.addSlot("FRIDAY", (int)WeekDay::FRIDAY);
        enm.addSlot("SATURDAY", (int)WeekDay::SATURDAY);
        enm.addSlot("SUNDAY", (int)WeekDay::SUNDAY);

        // Compile script and run it
        SqScript script = vm.compileFile("example_enums.nut");

        // Run the script
        vm.run(script);

        SqFunction getThirdDay = vm.findFunc("getThirdDay");
        SqObject firstDay = vm.callFunc(getThirdDay, vm);

        // Prints true
        std::cout << "getThirdDay " << (firstDay.toInt() == (int)WeekDay::WEDNESDAY ? "true" : "false") << std::endl;

        SqFunction isWednesday = vm.findFunc("isWednesday");
        bool ret = vm.callFunc(isWednesday, vm, (int)WeekDay::WEDNESDAY).toBool();

        // Prints true
        std::cout << "isWednesday: " << (ret ? "true" : "false") << std::endl;

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
