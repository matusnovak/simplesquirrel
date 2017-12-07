#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
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
