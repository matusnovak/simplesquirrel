#include <simplesquirrel/simplesquirrel.hpp>
#include <iomanip>
#include <algorithm>

class CppClass {
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

    static ssq::Class expose(ssq::VM& vm) {
        // auto cls = vm.addClass("CppClass", ssq::Class::Ctor<CppClass(std::string, int)>());
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
    ssq::VM vm(1024, ssq::Libs::STRING | ssq::Libs::MATH);

    try {
        // Compile script and run it
        ssq::Script script = vm.compileFile("example_classes.nut");

        // Expose the class
		CppClass::expose(vm);

        // Run the script
        vm.run(script);

        // We run the script, now we will getRaw getInstance and doStuff functions
        ssq::Function getInstance = vm.findFunc("getInstance");
        ssq::Function doStuff = vm.findFunc("doStuff");
        ssq::Function foo = vm.findFunc("foo");
        
        // We will getRaw copy of the instance allocated in the squirrel
        CppClass cpy = vm.callFunc(getInstance, vm).to<CppClass>();

        std::cout << "We got copy of CppClass instance and contains message: " << cpy.getMessage() << std::endl;

        // Send it back to squirrel to do some "stuff"
        // We will send it via a copy (no pointers)
        vm.callFunc(doStuff, vm, cpy);

        // Call foo function to print out the instance message inside of squirrel
        vm.callFunc(foo, vm);

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