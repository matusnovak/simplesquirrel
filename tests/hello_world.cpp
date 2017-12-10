#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#define SQUIRREL_STATIC
#include "../include/squirrelbind/squirrelbind.hpp"

#define STRINGIFY(x) #x

using namespace SquirrelBind;

TEST_CASE("Create virtual machine"){
    SqVM sq(1024, SqLibs::ALL);

    SqVM moved = std::move(sq);

    SqVM swapped(1024, SqLibs::ALL);

    std::swap(moved, swapped);

    swapped.destroy();
    moved.destroy();
    // Do nothing, 
    // should fall out of scope with no errors
}

TEST_CASE("Compile from source") {
    static const std::string source = STRINGIFY(
        class Foo {
            bar = "Hello World!";
        };
        
        local foo = Foo();
        print("Message: " + foo.bar);
    );

    SqVM vm(1024, SqLibs::ALL);

    SqScript script = vm.compileSource(source.c_str());

    REQUIRE(script.isEmpty() == false);
}

TEST_CASE("Compile from source and throw exception") {
    static const std::string source = STRINGIFY(
        class Foo {
            bar = "Hello World!";
        };
        
        local Foo foo();
        print("Message: " + foo.bar);
    );

    SqVM vm(1024, SqLibs::ALL);

    REQUIRE_THROWS_AS(vm.compileSource(source.c_str()), SqCompileException);
}

TEST_CASE("Compile from source and run") {
    static const std::string source = STRINGIFY(
        class Foo {
            bar = "Hello World!";
        };
        
        local foo = Foo();
        print("Message: " + foo.bar);
    );

    SqVM vm(1024, SqLibs::ALL);

    SqScript script = vm.compileSource(source.c_str());

    REQUIRE(script.isEmpty() == false);

    vm.run(script);
}

TEST_CASE("Run empty script") {
    SqVM vm(1024, SqLibs::ALL);

    SqScript script(vm.getHandle());

    REQUIRE(script.isEmpty() == true);

    REQUIRE_THROWS_AS(vm.run(script), SqRuntimeException);
}

TEST_CASE("Compile and run with exception"){
    static const std::string source = STRINGIFY(
        function add(a, b){
            return a + c;
        }
        add(a, b);
    );

    SqVM vm(1024, SqLibs::ALL);

    SqScript script = vm.compileSource(source.c_str());

    REQUIRE_THROWS_AS(vm.run(script), SqRuntimeException);
}