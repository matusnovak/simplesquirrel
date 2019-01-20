#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <simplesquirrel/simplesquirrel.hpp>

#define STRINGIFY(x) #x

TEST_CASE("Test object properties"){
    ssq::Object object;

    REQUIRE(object.isEmpty() == true);
}

TEST_CASE("Find object") {
    static const std::string source = STRINGIFY(
        class Foo {\n
            bar = "Hello World!";\n
        };\n
        \n
        test <- "String";
        local foo = Foo();\n
        print("Message: " + foo.bar);\n
    );

    ssq::VM vm(1024, ssq::Libs::ALL);

    REQUIRE(vm.getType() == ssq::Type::TABLE);

    ssq::Script script = vm.compileSource(source.c_str());
    REQUIRE(script.isEmpty() == false);
    vm.run(script);

    auto top = vm.getTop();

    ssq::Object found = vm.find("Foo");

    REQUIRE(top == vm.getTop());
    REQUIRE(found.isEmpty() == false);

    ssq::Type type = found.getType();

    REQUIRE(top == vm.getTop());
    REQUIRE(type == ssq::Type::CLASS);
}

TEST_CASE("Find class and method") {
    static const std::string source = STRINGIFY(
        class Foo {
            bar = "Hello World!";

            function baz(a, b) {
                return a + b;
            }
        };
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Class cls = vm.findClass("Foo");

    REQUIRE(cls.isEmpty() == false);

    ssq::Function baz = cls.findFunc("baz");

    REQUIRE(baz.isEmpty() == false);
}

TEST_CASE("Test stack manipulation") {
    static const std::string source = STRINGIFY(
        class Foo {
            constructor(v) {
                bar = v;
            }

            function baz(a, b) {
                return a + b;
            }

            bar = "Hello World!";
        };

        function test(a, b) {
            return a + b;
        }

        local v = null;
        function setVal(val) {
            v = val;
        }

        function getVal() {
            return v;
        }
    );

    ssq::VM vm(1024);

    static auto top = vm.getTop();

    ssq::Script script = vm.compileSource(source.c_str());
    REQUIRE(top == vm.getTop());

    vm.run(script);
    REQUIRE(top == vm.getTop());

    ssq::Function funcTest = vm.findFunc("test");
    REQUIRE(top == vm.getTop());

    ssq::Class classFoo = vm.findClass("Foo");
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.findFunc("invalid"));
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.findClass("invalid"));
    REQUIRE(top == vm.getTop());

    ssq::Table table = vm.newTable();
    REQUIRE(top == vm.getTop());

    ssq::Array arr = vm.newArray();
    REQUIRE(top == vm.getTop());

    funcTest.getNumOfParams();
    REQUIRE(top == vm.getTop());

    ssq::Instance inst = vm.newInstance(classFoo, std::string("Banana"));
    REQUIRE(top == vm.getTop());

    ssq::Function funcFooBaz = classFoo.findFunc("baz");
    REQUIRE(top == vm.getTop());

    ssq::Function funcCopy = funcFooBaz;
    REQUIRE(top == vm.getTop());

    ssq::Object ret = vm.callFunc(funcFooBaz, inst, 10, 20);
    REQUIRE(top == vm.getTop());

    ssq::Object retCopy = ret;
    REQUIRE(top == vm.getTop());

    int retInt = ret.toInt();
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.callFunc(funcFooBaz, inst, 1, 2, 3, 4, 5));
    REQUIRE(top == vm.getTop());

    ssq::Enum enm = vm.addEnum("Fruit");
    REQUIRE(top == vm.getTop());

    enm.addSlot("BANANA", 0);
    enm.addSlot("ORANGE", 1);
    enm.addSlot("APPLE", 2);
    enm.addSlot("LEMON", 3);
    REQUIRE(top == vm.getTop());

    ssq::Enum enmCopy = enm;
    REQUIRE(top == vm.getTop());

    table.set("one", 123);
    table.set("two", 1456);
    REQUIRE(top == vm.getTop());

    auto tableSize = table.size();
    REQUIRE(top == vm.getTop());

    ssq::Table copyRef = table;
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(table.get<int>("invalid"));
    REQUIRE(top == vm.getTop());

    int one = table.get<int>("one");
    int two = table.get<int>("two");
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(table.get<std::string>("one"));
    REQUIRE(top == vm.getTop());

    struct Car {
        std::string manufacturer;
        int year;
        std::string color;

        static ssq::Class expose(ssq::VM& vm) {
            ssq::Class cls = vm.addClass("Car", [](std::string man, int y, std::string col) -> Car* {
                auto car = new Car;
                car->manufacturer = man;
                car->year = y;
                car->color = col;
                return car;
            });

            REQUIRE(top == vm.getTop());

            cls.addVar("manufacturer", &Car::manufacturer);
            cls.addVar("year", &Car::year);
            cls.addVar("color", &Car::color);

            REQUIRE(top == vm.getTop());

            return cls;
        }
    };

	Car::expose(vm);
    REQUIRE(top == vm.getTop());

    ssq::Class carClass = vm.findClass("Car");
    REQUIRE(top == vm.getTop());

    ssq::Instance carInstance = vm.newInstance(carClass, std::string("Squirrel"), 2017, std::string("Blue"));
    REQUIRE(top == vm.getTop());

    ssq::Array array = vm.newArray();
    REQUIRE(top == vm.getTop());

    array.size();
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(array.back<int>());
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(array.begin<int>());
    REQUIRE(top == vm.getTop());

    array.push(std::string("Hello"));
    REQUIRE(top == vm.getTop());

    REQUIRE(array.get<std::string>(0) == "Hello");
    REQUIRE(top == vm.getTop());

    array.pop();
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(array.pop());
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(array.get<std::string>(0));
    REQUIRE(top == vm.getTop());

    class Bar {
    public:
        Bar(const std::string& name):name(name) {
            
        }

        std::string name;
    };

    std::unique_ptr<Bar> ptr(new Bar("Hello"));

    ssq::Function funcSetVal = vm.findFunc("setVal");
    ssq::Function funcGetVal = vm.findFunc("getVal");
    REQUIRE(top == vm.getTop());

    vm.callFunc(funcSetVal, vm, ptr.get());
    REQUIRE(top == vm.getTop());

    ret = vm.callFunc(funcGetVal, vm);
    REQUIRE(ret.getType() == ssq::Type::USERPOINTER);
    REQUIRE(top == vm.getTop());

    Bar* ptrTest = ret.to<Bar*>();
    REQUIRE(ptrTest == ptr.get());
    REQUIRE(top == vm.getTop());
}

TEST_CASE("Test nullptr") {
    static const std::string source = STRINGIFY(
        local v = null;
        function setVal(val) {
            v = val;
        }

        function getVal() {
            return v;
        }

        function getType() {
            return typeof v;
        }
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGetVal = vm.findFunc("getVal");
    ssq::Function funcSetVal = vm.findFunc("setVal");
    ssq::Function funcGetType = vm.findFunc("getType");

    vm.callFunc(funcSetVal, vm, nullptr);

    ssq::Object ret = vm.callFunc(funcGetVal, vm);

    REQUIRE(ret.isNull() == true);
    REQUIRE(ret.getType() == ssq::Type::NULLPTR);

    auto type = vm.callFunc(funcGetType, vm).toString();
    REQUIRE(type == "null");
}

TEST_CASE("Test param packer") {
    char ptr[64];

    ssq::detail::paramPacker<int>(ptr);
    REQUIRE(std::string(ptr) == "i");

    ssq::detail::paramPacker<const int>(ptr);
    REQUIRE(std::string(ptr) == "i");

    ssq::detail::paramPacker<int&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    ssq::detail::paramPacker<const int&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    ssq::detail::paramPacker<int const&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    ssq::detail::paramPacker<std::string>(ptr);
    REQUIRE(std::string(ptr) == "s");

    ssq::detail::paramPacker<const std::string>(ptr);
    REQUIRE(std::string(ptr) == "s");

    ssq::detail::paramPacker<std::string&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    ssq::detail::paramPacker<const std::string&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    ssq::detail::paramPacker<std::string const&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    ssq::detail::paramPacker<ssq::Object>(ptr);
    REQUIRE(std::string(ptr) == ".");

    ssq::detail::paramPacker<ssq::Instance>(ptr);
    REQUIRE(std::string(ptr) == "x");

    ssq::detail::paramPacker<ssq::Class>(ptr);
    REQUIRE(std::string(ptr) == "y");

    ssq::detail::paramPacker<ssq::Table>(ptr);
    REQUIRE(std::string(ptr) == "t");

    ssq::detail::paramPacker<ssq::Array>(ptr);
    REQUIRE(std::string(ptr) == "a");

    ssq::detail::paramPacker<ssq::Function>(ptr);
    REQUIRE(std::string(ptr) == "c");

    ssq::detail::paramPacker<std::nullptr_t>(ptr);
    REQUIRE(std::string(ptr) == "o");
}
