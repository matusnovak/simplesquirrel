#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#include "../include/squirrelbind/squirrelbind.hpp"

#define STRINGIFY(x) #x

using namespace SquirrelBind;

TEST_CASE("Find class"){
    static const std::string source = STRINGIFY(
		class Vector {
			constructor(x, y, z) {
				this.x = x;
				this.y = y;
				this.z = z;
			}

			function getX() {
				return x;
			}

			function getY() {
				return y;
			}

			function getZ() {
				return z;
			}
			
			x = 0;
			y = 0;
			z = 0;
		};
	);

	SqVM vm(1024, SqLibs::ALL);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	SqClass vectorClass = vm.findClass("Vector");
	SqInstance vector = vm.newInstance(vectorClass, 5, 10, 15);

	REQUIRE(vector.isEmpty() == false);

	SqFunction getX = vectorClass.findFunc("getX");
	SqFunction getY = vectorClass.findFunc("getY");
	SqFunction getZ = vectorClass.findFunc("getZ");

	REQUIRE(vm.callFunc(getX, vector).asInt() == 5);
	REQUIRE(vm.callFunc(getY, vector).asInt() == 10);
	REQUIRE(vm.callFunc(getZ, vector).asInt() == 15);
}

TEST_CASE("Register class") {
	class Foo {
	public:
		Foo(std::string msg):msg(msg) {
			
		}

		std::string getMsg() const {
			return msg;
		}

		std::string msg;
	};
	
	static const std::string source = STRINGIFY(
		local instance = null;
		
		function bar() {
			instance = Foo("Hello World");
		}
	);

	SqVM vm(1024, SqLibs::ALL);
	SqScript script = vm.compileSource(source.c_str());
	vm.run(script);

	vm.addClass("Foo", SqClass::Ctor<Foo(std::string)>());
}
