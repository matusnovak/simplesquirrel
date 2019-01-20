#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <simplesquirrel/simplesquirrel.hpp>

#define STRINGIFY(x) #x

TEST_CASE("Create virtual machine"){
    ssq::VM sq(1024, ssq::Libs::ALL);

    ssq::VM moved = std::move(sq);

    ssq::VM swapped(1024, ssq::Libs::ALL);

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

    ssq::VM vm(1024, ssq::Libs::ALL);

    ssq::Script script = vm.compileSource(source.c_str());

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

    ssq::VM vm(1024, ssq::Libs::ALL);

    REQUIRE_THROWS_AS(vm.compileSource(source.c_str()), ssq::CompileException);
}

TEST_CASE("Compile from source and run") {
    static const std::string source = STRINGIFY(
        class Foo {
            bar = "Hello World!";
        };
        
        local foo = Foo();
        print("Message: " + foo.bar);
    );

    ssq::VM vm(1024, ssq::Libs::ALL);

    ssq::Script script = vm.compileSource(source.c_str());

    REQUIRE(script.isEmpty() == false);

    vm.run(script);
}

TEST_CASE("Run empty script") {
    ssq::VM vm(1024, ssq::Libs::ALL);

    ssq::Script script(vm.getHandle());

    REQUIRE(script.isEmpty() == true);

    REQUIRE_THROWS_AS(vm.run(script), ssq::RuntimeException);
}

TEST_CASE("Compile and run with exception"){
    static const std::string source = STRINGIFY(
        function add(a, b){
            return a + c;
        }
        add(a, b);
    );

    ssq::VM vm(1024, ssq::Libs::ALL);

    ssq::Script script = vm.compileSource(source.c_str());

    REQUIRE_THROWS_AS(vm.run(script), ssq::RuntimeException);
}