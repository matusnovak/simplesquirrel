#define SQUIRREL_STATIC
#include <squirrelbind/squirrelbind.hpp>
#include <iomanip>
#include <algorithm>

using namespace SquirrelBind;

class CppClass: public SqClassWrapper<CppClass> {
public:
    CppClass(const std::string& message, int integer):message(message),integer(integer) {
        
    }

    void setMessage(const std::string& message) {
        this->message = message;
    }

    const std::string& getMessage() const {
        return message;
    }

    int getInteger() const {
        return integer;
    }

    static SqClass expose(SqVM& vm) {
        // auto cls = vm.addClass("CppClass", SqClass::Ctor<CppClass(std::string, int)>());
        // Alternative approach using lambda:
        auto cls = vm.addClass("CppClass", [](std::string message, int integer) -> CppClass* {
            return new CppClass(message, integer);
        });

        cls.addFunc("setMessage", &CppClass::setMessage);
        cls.addFunc("getMessage", &CppClass::getMessage);
        cls.addFunc("getInteger", &CppClass::getInteger);
        cls.addFunc("getMessageLowerCase", [](CppClass* self) -> std::string {
            std::string data = self->message;
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            return data;
        });
        cls.addVar("message", &CppClass::message);
        cls.addConstVar("integer", &CppClass::integer);

        return cls;
    }

    // We will leave them public so Squirrel 
    // can access them for the purpose of this example
    std::string message;
    const int integer;
};

int main() {
    // Create VM with stack size of 1024 and load string and math libraries
    SqVM vm(1024, SqLibs::STRING | SqLibs::MATH);

    try {
        // Compile script and run it
        SqScript script = vm.compileFile("example_classes.nut");

        // Expose the class
        vm.expose<CppClass>();

        // Run the script
        vm.run(script);

        // We run the script, now we will getRaw getInstance and doStuff functions
        SqFunction getInstance = vm.findFunc("getInstance");
        SqFunction doStuff = vm.findFunc("doStuff");
        SqFunction foo = vm.findFunc("foo");
        
        // We will getRaw copy of the instance allocated in the squirrel
        CppClass cpy = vm.callFunc(getInstance, vm).to<CppClass>();

        std::cout << "We got copy of CppClass instance and contains message: " << cpy.getMessage() << std::endl;

        // Send it back to squirrel to do some "stuff"
        // We will send it via a copy (no pointers)
        vm.callFunc(doStuff, vm, cpy);

        // Call foo function to print out the instance message inside of squirrel
        vm.callFunc(foo, vm);

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