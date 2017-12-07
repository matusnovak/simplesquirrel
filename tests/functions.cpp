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

    REQUIRE(ret.toInt() == 30);
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

    SqFunction foo = vm.callFunc(bar, vm).toFunction();

    int ret = vm.callFunc(foo, vm, 8, 4).toInt();

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

    SqFunction lambda = vm.callFunc(bar, vm).toFunction();

    int ret = vm.callFunc(lambda, vm, 8, 4).toInt();

    REQUIRE(ret == 12);
}

TEST_CASE("Register C++ func and call from squirrel") {
	static const std::string source = STRINGIFY(
		local result = foo(10, 20);
		function getResult() {
			return result;
		}
    );

	SqVM vm(1024);

	vm.addFunc("foo", std::function<int(int, int)>([](int a, int b){
		return a + b;
	}));

	SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

	SqFunction getResult = vm.findFunc("getResult");

	int result = vm.callFunc(getResult, vm).to<int>();

	REQUIRE(result == 30);
}

TEST_CASE("Register C++ lambda and call from squirrel") {
	static const std::string source = STRINGIFY(
		local result = foo(10, 20);
		function getResult() {
			return result;
		}
    );

	SqVM vm(1024);

	vm.addFunc("foo", [](int a, int b) -> std::string {
		return std::to_string(a + b);
	});

	SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

	SqFunction getResult = vm.findFunc("getResult");

	auto result = vm.callFunc(getResult, vm).toString();

	REQUIRE(result == "30");
}

template<typename T>
static void testType(T value, const std::string& type) {
	static const std::string source = STRINGIFY(
		local result = null;
		function foo(value) {
			result = value;
		}
		function bar() {
			return result;
		}
		function baz() {
			return typeof result;
		}
    );

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

	SqFunction foo = vm.findFunc("foo");
	SqFunction bar = vm.findFunc("bar");
	SqFunction baz = vm.findFunc("baz");

	vm.callFunc(foo, vm, value);

	SqObject result = vm.callFunc(bar, vm);
	REQUIRE(result.isEmpty() == false);

	REQUIRE(value == Approx(result.to<T>()));

#ifdef SQUNICODE
	std::wstring returnedType = vm.callFunc(baz, vm).toString();
#else
	std::string returnedType = vm.callFunc(baz, vm).toString();
#endif
	REQUIRE(returnedType == type);
}

#ifdef SQUNICODE
template<>
static void testType(std::wstring value, const std::string& type) {
#else
template<>
static void testType(std::string value, const std::string& type) {
#endif
	static const std::string source = STRINGIFY(
		local result = null;
		function foo(value) {
			result = value;
		}
		function bar() {
			return result;
		}
		function baz() {
			return typeof result;
		}
    );

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

	SqFunction foo = vm.findFunc("foo");
	SqFunction bar = vm.findFunc("bar");
	SqFunction baz = vm.findFunc("baz");

	vm.callFunc(foo, vm, value);

	SqObject result = vm.callFunc(bar, vm);
	REQUIRE(result.isEmpty() == false);

#ifdef SQUNICODE
	REQUIRE(value == result.to<std::wstring>());

	std::wstring returnedType = vm.callFunc(baz, vm).toString();
#else
	REQUIRE(value == result.to<std::string>());

	std::string returnedType = vm.callFunc(baz, vm).toString();
#endif
	REQUIRE(returnedType == type);
}

TEST_CASE("Test passing int") {
	testType<int>(30504030, "integer");
}

TEST_CASE("Test passing short") {
	testType<short>(25050, "integer");
}

TEST_CASE("Test passing char") {
	testType<char>(120, "integer");
}

TEST_CASE("Test passing long") {
	testType<long>(10203040, "integer");
}

TEST_CASE("Test passing unsigned int") {
	testType<unsigned int>(0xF7426358, "integer");
}

TEST_CASE("Test passing unsigned short") {
	testType<unsigned short>(0xFABC, "integer");
}

TEST_CASE("Test passing unsigned char") {
	testType<unsigned char>(0xFA, "integer");
}

TEST_CASE("Test passing unsigned long") {
	testType<unsigned long>(0xFF429587, "integer");
}

TEST_CASE("Test passing float") {
	testType<float>(123.456f, "float");
}

TEST_CASE("Test passing int32_t") {
	testType<int32_t>(30504030, "integer");
}

TEST_CASE("Test passing int16_t") {
	testType<int16_t>(25050, "integer");
}

TEST_CASE("Test passing int8_t") {
	testType<int8_t>(120, "integer");
}

TEST_CASE("Test passing uint32_t") {
	testType<uint32_t>(0xF7426358, "integer");
}

TEST_CASE("Test passing uint16_t") {
	testType<uint16_t>(0xFABC, "integer");
}

TEST_CASE("Test passing uint8_t") {
	testType<uint8_t>(0xFA, "integer");
}

#ifdef SQUSEDOUBLE
TEST_CASE("Test passing double") {
	testType<double>(0.122554578.0, "float");
}
#endif

#ifdef _SQ64
TEST_CASE("Test passing unsigned long long") {
	testType<unsigned long long>(0xFFCCBBAAFF429587, "integer");
}

TEST_CASE("Test passing long long") {
	testType<long long>(0x12746585FF429587, "integer");
}

TEST_CASE("Test passing uint64_t") {
	testType<uint64_t>(0xFFCCBBAAFF429587, "integer");
}

TEST_CASE("Test passing int64_t") {
	testType<int64_t>(0x12746585FF429587, "integer");
}
#endif

#ifdef SQUNICODE
TEST_CASE("Test passing std::wstring") {
	testType<std::wstring>(L"Hello World!", "string");
}
#else
TEST_CASE("Test passing std::string") {
	testType<std::string>("Hello World!", "string");
}
#endif

TEST_CASE("Pass raw object") {
	struct Data {
		int myInteger;
		float myFloat;
		std::string myString;
	};

	static const std::string source = STRINGIFY(
		local result = null;
		function set(value) {
			result = value;
		}
		function get() {
			return result;
		}
		function getType() {
			return typeof result;
		}
    );

	SqVM vm(1024);
	SqScript script = vm.compileSource(source.c_str());
    vm.run(script);

	SqFunction funcGet = vm.findFunc("get");
	SqFunction funcSet = vm.findFunc("set");
	SqFunction funcGetType = vm.findFunc("getType");

	Data data{42, 123.456f, "Banana"};

	vm.callFunc(funcSet, vm, data);
}
