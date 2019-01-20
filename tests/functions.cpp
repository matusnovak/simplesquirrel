#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <simplesquirrel/simplesquirrel.hpp>

#define STRINGIFY(x) #x

TEST_CASE("Find and call function"){
    static const std::string source = STRINGIFY(
        function foo(a, b) {
            return a + b;
        }
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Object found = vm.find("foo");
    
    REQUIRE(found.isEmpty() == false);
    REQUIRE(found.getType() == ssq::Type::CLOSURE);

    ssq::Function func = vm.findFunc("foo");

    REQUIRE(func.isEmpty() == false);
    REQUIRE(func.getType() == ssq::Type::CLOSURE);
    REQUIRE(func.getNumOfParams() == 2);

    ssq::Object ret = vm.callFunc(func, vm, 10, 20);

    REQUIRE(ret.isEmpty() == false);
    REQUIRE(ret.getType() == ssq::Type::INTEGER);

    REQUIRE(ret.toInt() == 30);
}

TEST_CASE("Call pure void function") {
    static const std::string source = STRINGIFY(
        function bar() {
            print("Pure void!");
        }
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function bar = vm.findFunc("bar");
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

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function bar = vm.findFunc("bar");
    REQUIRE(bar.isEmpty() == false);

    ssq::Function foo = vm.callFunc(bar, vm).toFunction();

    int ret = vm.callFunc(foo, vm, 8, 4).toInt();

    REQUIRE(ret == 12);
}

TEST_CASE("Call function and return lambda") {
    static const std::string source = STRINGIFY(
        function bar() {
            return @(a,b) a + b;
        }
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function bar = vm.findFunc("bar");
    REQUIRE(bar.isEmpty() == false);

    ssq::Function lambda = vm.callFunc(bar, vm).toFunction();

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

    ssq::VM vm(1024);

    vm.addFunc("foo", std::function<int(int, int)>([](int a, int b){
        return a + b;
    }));

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function getResult = vm.findFunc("getResult");

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

    ssq::VM vm(1024);

    vm.addFunc("foo", [](int a, int b) -> std::string {
        return std::to_string(a + b);
    });

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function getResult = vm.findFunc("getResult");

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

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function foo = vm.findFunc("foo");
    ssq::Function bar = vm.findFunc("bar");
    ssq::Function baz = vm.findFunc("baz");

    vm.callFunc(foo, vm, value);

    ssq::Object result = vm.callFunc(bar, vm);
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
void testType(std::wstring value, const std::string& type) {
#else
template<>
void testType(std::string value, const std::string& type) {
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

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function foo = vm.findFunc("foo");
    ssq::Function bar = vm.findFunc("bar");
    ssq::Function baz = vm.findFunc("baz");

    vm.callFunc(foo, vm, value);

    ssq::Object result = vm.callFunc(bar, vm);
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

TEST_CASE("Pass class object as user data") {
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

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcSet = vm.findFunc("set");
    ssq::Function funcGetType = vm.findFunc("getType");

    Data data{42, 123.456f, "Banana"};

    vm.callFunc(funcSet, vm, data);

    auto type = vm.callFunc(funcGetType, vm).toString();
    REQUIRE(type == "userdata");

    Data ret = vm.callFunc(funcGet, vm).to<Data>();

    REQUIRE(data.myInteger == ret.myInteger);
    REQUIRE(data.myFloat == Approx(ret.myFloat));
    REQUIRE(data.myString == ret.myString);
}

TEST_CASE("Pass class object as copy instance") {
    class Foo {
    public:
        Foo(const std::string& value):value(value) {
        }

        ~Foo() {
            
        }

        const std::string& getValue() const {
            return value;
        }

        void setValue(const std::string& value) {
            this->value = value;
        }

        static void expose(ssq::VM& vm) {
            ssq::Class cls = vm.addClass("Foo", ssq::Class::Ctor<Foo(std::string)>());

            cls.addFunc("getValue", &Foo::getValue);
            cls.addFunc("setValue", &Foo::setValue);
        }

        std::string value;
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

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

	Foo::expose(vm);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcSet = vm.findFunc("set");
    ssq::Function funcGetType = vm.findFunc("getType");

    Foo foo("Hello World");

    vm.callFunc(funcSet, vm, foo);

    auto type = vm.callFunc(funcGetType, vm).toString();
    REQUIRE(type == "instance");

    Foo ret = vm.callFunc(funcGet, vm).to<Foo>();

    REQUIRE(ret.getValue() == foo.getValue());
}

TEST_CASE("Return userdata") {
    struct Data {
        int myInteger;
        float myFloat;
        std::string myString;
    };

    static const std::string source = STRINGIFY(
        local result = null;
        function get() {
            return result;
        }
        function getType() {
            return typeof result;
        }

        result = foo();
    );

    ssq::VM vm(1024);

    vm.addFunc("foo", []() -> Data {
        return Data{42, 123.456f, "Banana"};
    });

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcGetType = vm.findFunc("getType");

    auto type = vm.callFunc(funcGetType, vm).toString();

    REQUIRE(type == "userdata");

    Data ret = vm.callFunc(funcGet, vm).to<Data>();
    
    REQUIRE(ret.myInteger == 42);
    REQUIRE(ret.myFloat == Approx(123.456f));
    REQUIRE(ret.myString == "Banana");
}

TEST_CASE("Return instance") {
    class Foo {
    public:
        Foo(const std::string& value):value(value) {
        }

        ~Foo() {
            
        }

        const std::string& getValue() const {
            return value;
        }

        void setValue(const std::string& value) {
            this->value = value;
        }

        static void expose(ssq::VM& vm) {
            ssq::Class cls = vm.addClass("Foo", ssq::Class::Ctor<Foo(std::string)>());

            cls.addFunc("getValue", &Foo::getValue);
            cls.addFunc("setValue", &Foo::setValue);
        }

        std::string value;
    };

    static const std::string source = STRINGIFY(
        local result = null;
        function get() {
            return result;
        }
        function getType() {
            return typeof result;
        }

        result = foo();
    );

    ssq::VM vm(1024);
	Foo::expose(vm);
    vm.addFunc("foo", []() -> Foo {
        return Foo("Banana");
    });

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcGetType = vm.findFunc("getType");

    auto type = vm.callFunc(funcGetType, vm).toString();
    REQUIRE(type == "instance");

    Foo ret = vm.callFunc(funcGet, vm).to<Foo>();

    REQUIRE(ret.getValue() == "Banana");
}

TEST_CASE("Test passing table") {
    static const std::string source = STRINGIFY(
        local dict = {
            test1=1234
            deleteme="now"
        };

        function set(val) {
            dict = val;
        }
        function get() {
            return dict;
        }
        function test(t) {
            if (t.aaa != 123) throw "t.aaa not 123!";
            if (t.bbb != true) throw "t.bbb not true!";
            if (t.ccc != "Hello") throw "t.ccc not \"Hello\"!";
            t.ddd <- "World";
            return t;
        }
    );

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcSet = vm.findFunc("set");
    ssq::Function funcTest = vm.findFunc("test");

    ssq::Object ret = vm.callFunc(funcGet, vm);
    REQUIRE(ret.getType() == ssq::Type::TABLE);

    ssq::Table table = ret.toTable();

    int test1 = table.get<int>("test1");
    std::string deleteme = table.get<std::string>("deleteme");

    REQUIRE(test1 == 1234);
    REQUIRE(deleteme == "now");
    REQUIRE(table.size() == 2);

    table.set("test1", 42);

    REQUIRE(table.size() == 2);

    test1 = table.get<int>("test1");

    REQUIRE(test1 == 42);

    ssq::Table newTable = vm.newTable();

    newTable.set("aaa", 123);
    newTable.set("bbb", true);
    newTable.set("ccc", std::string("Hello"));

    ssq::Table result = vm.callFunc(funcTest, vm, newTable).toTable();

    REQUIRE(result.size() == 4);
    REQUIRE(result.get<std::string>("ddd") == "World");
}

TEST_CASE("Test passing array") {
    static const std::string source =
        "local arr = [123, true, \"Hello World!\"];\n"
        "\n"
        "function set(val) {\n"
        "	arr = val;\n"
        "}\n"
        "function foo() {\n"
        "	return 42;\n"
        "}\n"
        "\n"
        "function get() {\n"
        "	return arr;\n"
        "}\n"
        "function test(a) {\n"
        "	if (a[0] != \"Banana\") throw \"a[0] is not \\\"Banana\\\"\";\n"
        "	if (a[1] != \"Orange\") throw \"a[1] is not \\\"Orange\\\"\";\n"
        "	if (a[2]() != 42) throw \"a[2] is not a function\";\n"
        "}\n"
        "function test2(a) {\n"
        "	if (a[0] != \"Banana\") throw \"a[0] is not \\\"Banana\\\"\";\n"
        "	if (a[1] != \"Orange\") throw \"a[1] is not \\\"Orange\\\"\";\n"
        "}\n";

    ssq::VM vm(1024);
    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);

    ssq::Function funcGet = vm.findFunc("get");
    ssq::Function funcSet = vm.findFunc("set");
    ssq::Function funcFoo = vm.findFunc("foo");
    ssq::Function funcTest = vm.findFunc("test");
    ssq::Function funcTest2 = vm.findFunc("test2");

    ssq::Object ret = vm.callFunc(funcGet, vm);

    REQUIRE(ret.getType() == ssq::Type::ARRAY);

    ssq::Array arr = ret.toArray();

    REQUIRE(arr.size() == 3);

    std::string a2 = arr.popAndGet<std::string>();
    REQUIRE(a2 == "Hello World!");

    bool a1 = arr.popAndGet<bool>();
    REQUIRE(a1 == true);

    int a0 = arr.popAndGet<int>();
    REQUIRE(a0 == 123);

    arr.push(std::string("Banana"));
    arr.push(std::string("Orange"));
    arr.push(funcFoo);

    vm.callFunc(funcTest, vm, arr);

    std::vector<std::string> vec;
    vec.push_back("Banana");
    vec.push_back("Orange");

    ssq::Array vecArr = vm.newArray(vec);
    vm.callFunc(funcTest2, vm, vecArr);
}

TEST_CASE("Test passing instance") {
	class GuiButton;
	static GuiButton* buttonPtr = nullptr;
	
	class GuiButton {
	public:
		GuiButton(const std::string& label):label(label) {
			buttonPtr = this;
		}

		const std::string& getLabel() const {
			return label;
		}

		std::string label;
	};

	static const std::string source = STRINGIFY(
        class Foo extends GuiButton {
			constructor(label) {
				base.constructor(label);
			}
        }

		local instance = Foo("Hello");

		print("Calling bar");
		bar(instance);

		print("Calling baz");
		baz(instance);
    );

    ssq::VM vm(1024);

	ssq::Class cls = vm.addClass("GuiButton", ssq::Class::Ctor<GuiButton(std::string)>());
	cls.addFunc("getLabel", &GuiButton::getLabel);

	vm.addFunc("bar", [&](GuiButton* button) -> void {
		REQUIRE(button == buttonPtr);
	});

	vm.addFunc("baz", [&](ssq::Instance instance) -> void {
		REQUIRE(instance.toPtrUnsafe<GuiButton*>() == buttonPtr);
	});

    ssq::Script script = vm.compileSource(source.c_str());
    vm.run(script);
}
