#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#define SQUIRREL_STATIC
#include "../include/squirrelbind/squirrelbind.hpp"

#define STRINGIFY(x) #x

using namespace SquirrelBind;

TEST_CASE("Test object properties"){
    SqObject object;

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

    SqVM vm(1024, SqLibs::ALL);

    REQUIRE(vm.getType() == SqType::TABLE);

    SqScript script = vm.compileSource(source.c_str());
    REQUIRE(script.isEmpty() == false);
    vm.run(script);

    auto top = vm.getTop();

    SqObject found = vm.find("Foo");

    REQUIRE(top == vm.getTop());
    REQUIRE(found.isEmpty() == false);

    SqType type = found.getType();

    REQUIRE(top == vm.getTop());
    REQUIRE(type == SqType::CLASS);
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

    SqVM vm(1024);
    SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

    SqClass cls = vm.findClass("Foo");

    REQUIRE(cls.isEmpty() == false);

    SqFunction baz = cls.findFunc("baz");

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

    SqVM vm(1024);

    static auto top = vm.getTop();

    SqScript script = vm.compileSource(source.c_str());
    REQUIRE(top == vm.getTop());

    vm.run(script);
    REQUIRE(top == vm.getTop());

    SqFunction funcTest = vm.findFunc("test");
    REQUIRE(top == vm.getTop());

    SqClass classFoo = vm.findClass("Foo");
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.findFunc("invalid"));
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.findClass("invalid"));
    REQUIRE(top == vm.getTop());

    SqTable table = vm.newTable();
    REQUIRE(top == vm.getTop());

    SqArray arr = vm.newArray();
    REQUIRE(top == vm.getTop());

    funcTest.getNumOfParams();
    REQUIRE(top == vm.getTop());

    SqInstance inst = vm.newInstance(classFoo, std::string("Banana"));
    REQUIRE(top == vm.getTop());

    SqFunction funcFooBaz = classFoo.findFunc("baz");
    REQUIRE(top == vm.getTop());

    SqFunction funcCopy = funcFooBaz;
    REQUIRE(top == vm.getTop());

    SqObject ret = vm.callFunc(funcFooBaz, inst, 10, 20);
    REQUIRE(top == vm.getTop());

    SqObject retCopy = ret;
    REQUIRE(top == vm.getTop());

    int retInt = ret.toInt();
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(vm.callFunc(funcFooBaz, inst, 1, 2, 3, 4, 5));
    REQUIRE(top == vm.getTop());

    SqEnum enm = vm.addEnum("Fruit");
    REQUIRE(top == vm.getTop());

    enm.addSlot("BANANA", 0);
    enm.addSlot("ORANGE", 1);
    enm.addSlot("APPLE", 2);
    enm.addSlot("LEMON", 3);
    REQUIRE(top == vm.getTop());

    SqEnum enmCopy = enm;
    REQUIRE(top == vm.getTop());

    table.set("one", 123);
    table.set("two", 1456);
    REQUIRE(top == vm.getTop());

    auto tableSize = table.size();
    REQUIRE(top == vm.getTop());

    SqTable copyRef = table;
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(table.get<int>("invalid"));
    REQUIRE(top == vm.getTop());

    int one = table.get<int>("one");
    int two = table.get<int>("two");
    REQUIRE(top == vm.getTop());

    REQUIRE_THROWS(table.get<std::string>("one"));
    REQUIRE(top == vm.getTop());

    struct Car: public SqClassWrapper<Car> {
        std::string manufacturer;
        int year;
        std::string color;

        static SqClass expose(SqVM& vm) {
            SqClass cls = vm.addClass("Car", [](std::string man, int y, std::string col) -> Car* {
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

    vm.expose<Car>();
    REQUIRE(top == vm.getTop());

    SqClass carClass = vm.findClass("Car");
    REQUIRE(top == vm.getTop());

    SqInstance carInstance = vm.newInstance(carClass, std::string("Squirrel"), 2017, std::string("Blue"));
    REQUIRE(top == vm.getTop());

    SqArray array = vm.newArray();
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

    SqFunction funcSetVal = vm.findFunc("setVal");
    SqFunction funcGetVal = vm.findFunc("getVal");
    REQUIRE(top == vm.getTop());

    vm.callFunc(funcSetVal, vm, ptr.get());
    REQUIRE(top == vm.getTop());

    ret = vm.callFunc(funcGetVal, vm);
    REQUIRE(ret.getType() == SqType::USERPOINTER);
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

    SqVM vm(1024);
    SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

    SqFunction funcGetVal = vm.findFunc("getVal");
    SqFunction funcSetVal = vm.findFunc("setVal");
    SqFunction funcGetType = vm.findFunc("getType");

    vm.callFunc(funcSetVal, vm, nullptr);

    SqObject ret = vm.callFunc(funcGetVal, vm);

    REQUIRE(ret.isNull() == true);
    REQUIRE(ret.getType() == SqType::NULLPTR);

    auto type = vm.callFunc(funcGetType, vm).toString();
    REQUIRE(type == "null");
}

TEST_CASE("Test param packer") {
    char ptr[64];

    detail::paramPacker<int>(ptr);
    REQUIRE(std::string(ptr) == "i");

    detail::paramPacker<const int>(ptr);
    REQUIRE(std::string(ptr) == "i");

    detail::paramPacker<int&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    detail::paramPacker<const int&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    detail::paramPacker<int const&>(ptr);
    REQUIRE(std::string(ptr) == "i");

    detail::paramPacker<std::string>(ptr);
    REQUIRE(std::string(ptr) == "s");

    detail::paramPacker<const std::string>(ptr);
    REQUIRE(std::string(ptr) == "s");

    detail::paramPacker<std::string&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    detail::paramPacker<const std::string&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    detail::paramPacker<std::string const&>(ptr);
    REQUIRE(std::string(ptr) == "s");

    detail::paramPacker<SqObject>(ptr);
    REQUIRE(std::string(ptr) == ".");

    detail::paramPacker<SqInstance>(ptr);
    REQUIRE(std::string(ptr) == "x");

    detail::paramPacker<SqClass>(ptr);
    REQUIRE(std::string(ptr) == "y");

    detail::paramPacker<SqTable>(ptr);
    REQUIRE(std::string(ptr) == "t");

    detail::paramPacker<SqArray>(ptr);
    REQUIRE(std::string(ptr) == "a");

    detail::paramPacker<SqFunction>(ptr);
    REQUIRE(std::string(ptr) == "c");

    detail::paramPacker<std::nullptr_t>(ptr);
    REQUIRE(std::string(ptr) == "o");
}
