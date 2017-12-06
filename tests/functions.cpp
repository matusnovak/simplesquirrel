#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#include "../include/squirrelbind/squirrelbind.hpp"

#define STRINGIFY(x) #x

using namespace SquirrelBind;

TEST_CASE("Find and call function"){
	static const std::string source = STRINGIFY(
		function foo(a, b) {
			return a + b;
		}
	);

    SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	SqObject found = vm.find("foo");
	
	REQUIRE(found.isEmpty() == false);
	REQUIRE(found.getType() == SqType::CLOSURE);

	SqFunction func = vm.findFunc("foo");

	REQUIRE(func.isEmpty() == false);
	REQUIRE(func.getType() == SqType::CLOSURE);
	REQUIRE(func.getNumOfParams() == 2);

	SqObject ret = vm.callFunc(func, vm, 10, 20);

	REQUIRE(ret.isEmpty() == false);
	REQUIRE(ret.getType() == SqType::INTEGER);

	REQUIRE(ret.asInt() == 30);
}

TEST_CASE("Call pure void function") {
	static const std::string source = STRINGIFY(
		function bar() {
			print("Pure void!");
		}
	);

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	SqFunction bar = vm.findFunc("bar");
	REQUIRE(bar.isEmpty() == false);

	vm.callFunc(bar, vm);
}

TEST_CASE("Call function and return function") {
	static const std::string source = STRINGIFY(
		function foo(a, b) {
			return a + b;
		}

		function bar() {
			return foo;
		}
	);

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	SqFunction bar = vm.findFunc("bar");
	REQUIRE(bar.isEmpty() == false);

	SqObject foo = vm.callFunc(bar, vm);

	int ret = vm.callFunc(foo, vm, 8, 4).asInt();

	REQUIRE(ret == 12);
}

TEST_CASE("Call function and return lambda") {
	static const std::string source = STRINGIFY(
		function bar() {
			return @(a,b) a + b;
		}
	);

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	SqFunction bar = vm.findFunc("bar");
	REQUIRE(bar.isEmpty() == false);

	SqObject lambda = vm.callFunc(bar, vm);

	int ret = vm.callFunc(lambda, vm, 8, 4).asInt();

	REQUIRE(ret == 12);
}
