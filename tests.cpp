#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#include "include/simplesq.hpp"

TEST_CASE("Create virtual machine"){
    ssq::Machine vm(1024);
    // Do nothing, 
    // should fall out of scope with no errors
}

TEST_CASE("Compile without exception"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(a, b);";

    ssq::Machine vm(1024);
    auto t0 = vm.getTop();
    ssq::Script script = vm.compileFromData(src);
    auto t1 = vm.getTop();
    REQUIRE(t0 == t1);
}


TEST_CASE("Compile with exception"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "$^&*//**-+((a, b);";

    ssq::Machine vm(1024);

    REQUIRE_THROWS_AS(
        vm.compileFromData(src), 
        ssq::CompileException
    );
}

TEST_CASE("Run empty script and throw"){
    ssq::Machine vm(1024);
    // Empty script
    ssq::Script script(vm.getHandle());

    REQUIRE_THROWS_AS(
        vm.run(script), 
        ssq::RuntimeException
    );
}

TEST_CASE("Compile and run"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(1, 2);";

    ssq::Machine vm(1024);

    auto t0 = vm.getTop();
    ssq::Script script = vm.compileFromData(src);
    vm.run(script);
    auto t1 = vm.getTop();

    REQUIRE(t0 == t1);
}

TEST_CASE("Compile and run with exception"){
    const std::string src = 
    "function add(a, b){"
    "    return a + c;"
    "}"
    "add(a, b);";

    ssq::Machine vm(1024);

    ssq::Script script = vm.compileFromData(src);

    REQUIRE_THROWS_AS(
        vm.run(script),
        ssq::RuntimeException
    );
}

TEST_CASE("Register enum"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(Test.hello, Test.world);";

    ssq::Machine vm(1024);

    auto t0 = vm.getTop();
    auto enm = vm.addEnum("Test");
    enm.addSlot("hello", 10);
    enm.addSlot("world", 20);
    auto t1 = vm.getTop();

    REQUIRE(t0 == t1);

    ssq::Script script = vm.compileFromData(src);
    vm.run(script);
}

TEST_CASE("Register enum and get invalid"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(Test.hello, Test.invalid);";

    ssq::Machine vm(1024);

    auto enm = vm.addEnum("Test");
    enm.addSlot("hello", 10);
    enm.addSlot("world", 20);

    REQUIRE_THROWS_AS(
        vm.compileFromData(src),
        ssq::CompileException
    );
}

TEST_CASE("Register enum that lives out of scope"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(Test.hello, Test.world);";

    ssq::Machine vm(1024);

    {
        auto enm = vm.addEnum("Test");
        enm.addSlot("hello", 10);
        enm.addSlot("world", 20);
        // enm gets destroyed here
        // but is still alive in the VM
    }

    ssq::Script script = vm.compileFromData(src);
    vm.run(script);
}

TEST_CASE("Register global const"){
    const std::string src = 
    "function add(a, b){"
    "    return a + b;"
    "}"
    "add(SOME_GLOBAL_CONST, 20);";

    ssq::Machine vm(1024);
    vm.addConst("SOME_GLOBAL_CONST", 10);

    ssq::Script script = vm.compileFromData(src);
    vm.run(script);
}

TEST_CASE("Register global func and compile"){
    const std::string src = 
    "local ret = someFunc(10);";

    ssq::Machine vm(1024);

    std::function<int(int)> func = [](int a) {
        return a*a;
    };

    vm.addFunc("someFunc", func);

    ssq::Script script = vm.compileFromData(src);
    vm.run(script);
}

TEST_CASE("Find global func"){
    ssq::Machine vm(1024);

    std::function<int(int)> func = [](int a) {
        return a*a;
    };

    vm.addFunc("foo", func);

    auto t0 = vm.getTop();
    vm.findFunc("foo");
    auto t1 = vm.getTop();
    REQUIRE(t0 == t1);
}

TEST_CASE("Find global func from script"){
    const std::string src = 
    "function foo(a){"
    "    return a*a;"
    "}";

    ssq::Machine vm(1024);
    ssq::Script script = vm.compileFromData(src);

    REQUIRE_THROWS_AS(
        vm.findFunc("foo"),
        ssq::NotFoundException
    );

    vm.run(script);
    vm.findFunc("foo");
}

TEST_CASE("Find and call global func from script"){
    const std::string src = 
    "local test = 0;"
    "function foo(a){"
    "    test = a*a;"
    "}"
    "function bar(){"
    "    return test;"
    "}";

    ssq::Machine vm(1024);
    ssq::Script script = vm.compileFromData(src);

    vm.run(script);

    auto foo = vm.findFunc("foo");
    auto bar = vm.findFunc("bar");

    vm.callGlobal<void>(foo, 5);
    int ret = vm.callGlobal<int>(bar);

    REQUIRE(ret == 25);
}

TEST_CASE("Find and call global func from script with strings"){
    const std::string src = 
    "local test = \"\";"
    "function foo(a, b, c){"
    "    test = a + b + c;"
    "}"
    "function bar(){"
    "    return test;"
    "}";

    ssq::Machine vm(1024);
    ssq::Script script = vm.compileFromData(src);

    vm.run(script);

    auto foo = vm.findFunc("foo");
    auto bar = vm.findFunc("bar");

    vm.callGlobal<void>(foo, std::string("Hello "), std::string("World! "), 42);
    std::string ret = vm.callGlobal<std::string>(bar);

    REQUIRE(ret == "Hello World! 42");
}

TEST_CASE("Bind class and allocate") {
	static bool constructorCalled = false;
	static bool destructorCalled = false;

	class MyClass {
	public:
		MyClass() {
			constructorCalled = true;
		}
		~MyClass() {
			destructorCalled = true;
		}
	};

	const std::string src =
	"local test = MyClass();";

	ssq::Machine vm(1024);
    ssq::Script script = vm.compileFromData(src);
	vm.addClass<MyClass>("MyClass");

	vm.run(script);

	REQUIRE(constructorCalled == true);
	REQUIRE(destructorCalled == true);
}

TEST_CASE("Bind class and allocate with custom allocator") {
	static bool constructorCalled = false;
	static bool destructorCalled = false;
	static int constructorParam = 0;

	class MyClass {
	public:
		MyClass(int i){
			constructorCalled = true;
			constructorParam = i;
		}
		~MyClass() {
			destructorCalled = true;
		}
	};

	const std::string src =
		"local test = MyClass();";

	ssq::Machine vm(1024);
	ssq::Script script = vm.compileFromData(src);
	vm.addClass<MyClass>("MyClass", std::function<MyClass*(void)>([]() {
		return new MyClass(42);
	}));

	vm.run(script);

	REQUIRE(constructorCalled == true);
	REQUIRE(destructorCalled == true);
	REQUIRE(constructorParam == 42);
}

TEST_CASE("Bind class and pass instance to script") {
	static bool lamdaCalled = false;
	static bool constructorCalled = false;
	static bool destructorCalled = false;
	static int constructorParam = 0;

	class MyClass {
	public:
		MyClass(int i){
			constructorCalled = true;
			constructorParam = i;
		}
		~MyClass() {
			destructorCalled = true;
		}
	};

	static MyClass* passedInstancePointer = nullptr;

	const std::string src =
		"local test = myFunc(42);"
		"myPassFunc(test);";

	std::unique_ptr<MyClass> myClass;

	std::function<MyClass*(int)> myFunc = [&](int val){
		lamdaCalled = true;
		myClass.reset(new MyClass(val));
		return myClass.get();
	};

	std::function<void(MyClass*)> myPassFunc = [&](MyClass* ptr){
		passedInstancePointer = ptr;
	};

	ssq::Machine vm(1024);
	vm.addFunc("myFunc", myFunc);
	vm.addFunc("myPassFunc", std::function<void(MyClass*)>([&](MyClass* ptr){
		passedInstancePointer = ptr;
	}));

	vm.addClass<MyClass>("MyClass", std::function<MyClass*(void)>([]() {
		return new MyClass(10);
	}));
	ssq::Script script = vm.compileFromData(src);

	vm.run(script);

	REQUIRE(lamdaCalled == true);
	REQUIRE(constructorCalled == true);
	REQUIRE(destructorCalled == false);
	REQUIRE(constructorParam == 42);
	REQUIRE(myClass.get() == passedInstancePointer);
}

TEST_CASE("Bind class with method") {
	static int widthSet = 0;
	static int heightSet = 0;

	class Rectangle {
	public:
		Rectangle() {
			width = 0;
			height = 0;
		}

		void setSize(int width, int height) {
			this->width = width;
			this->height = height;
			widthSet = width;
			heightSet = height;
		}

		int getWidth() const {
			return width;
		}

		int getHeight() const {
			return height;
		}

		int width;
		int height;
	};

	const std::string src =
		"local rectangle = Rectangle();"
		"rectangle.setSize(100, 50);";

	ssq::Machine vm(1024);
	ssq::Class<Rectangle> rectangleClass = vm.addClass<Rectangle>("Rectangle");

	//std::function<void(Rectangle*, int, int)> setSizeFunc = std::mem_fn(&Rectangle::setSize);

	rectangleClass.addFunc("setSize", &Rectangle::setSize);

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(widthSet == 100);
	REQUIRE(heightSet == 50);
}

TEST_CASE("Bind class with method and accept const reference to string") {
	static std::string lastTitleSet;
	static std::string lastTitleGet;

	class Widget {
	public:
		Widget() {
			
		}
		~Widget() {
			
		}
		void setTitle(const std::string& str) {
			lastTitleSet = str;
			title = str;
		}
		const std::string& getTitle() const {
			return title;
		}
	private:
		std::string title;
	};

	const std::string src =
		"local widget = Widget();"
		"widget.setTitle(\"Hello World!\");"
		"local title = widget.getTitle();"
		"::lastTitleGet(title);";

	ssq::Machine vm(1024);
	ssq::Class<Widget> cls = vm.addClass<Widget>("Widget");
	cls.addFunc("setTitle", std::function<void(Widget*, const std::string&)>( std::mem_fn(&Widget::setTitle) ));
	cls.addFunc("getTitle", std::function<const std::string&(Widget*)>( std::mem_fn(&Widget::getTitle) ));

	vm.addFunc("lastTitleGet", std::function<void(std::string)>([&](std::string str){
		lastTitleGet = str;
	}));

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(lastTitleSet == "Hello World!");
	REQUIRE(lastTitleGet == "Hello World!");
}

TEST_CASE("Bind class with enum") {
	class MyClass {
	public:
		MyClass() {
			
		}
		~MyClass() {
			
		}
	};

	ssq::Machine vm(1024);
	ssq::Class<MyClass> cls = vm.addClass<MyClass>("MyClass");
	ssq::Enum enm = cls.addEnum("MyEnum");
	enm.addSlot("String", "Hello World");
	enm.addSlot("Number", 42);
	enm.addSlot("Float", 123.456f);

	static std::string gotString;
	static int gotNumber;
	static float gotFloat;

	vm.addFunc("passString", std::function<void(const std::string&)>([&](const std::string& val){
		gotString = val;
	}));

	vm.addFunc("passNumber", std::function<void(int)>([&](int val){
		gotNumber = val;
	}));

	vm.addFunc("passFloat", std::function<void(float)>([&](float val){
		gotFloat = val;
	}));

	const std::string src =
		"::passString(MyClass.MyEnum.String);"
		"::passNumber(MyClass.MyEnum.Number);"
		"::passFloat(MyClass.MyEnum.Float);";

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(gotString == "Hello World");
	REQUIRE(gotNumber == 42);
	REQUIRE(gotFloat == Approx(123.456f));
}

TEST_CASE("Bind class with const var") {
	class MyClass {
	public:
		MyClass() {
			
		}
		~MyClass() {
			
		}
	};

	ssq::Machine vm(1024);
	ssq::Class<MyClass> cls = vm.addClass<MyClass>("MyClass");
	cls.addConst("MyConstString", std::string("Hello World"));
	cls.addConst("MyConstNumber", 42);
	cls.addConst("MyConstFloat", 123.456f);

	static std::string gotString;
	static int gotNumber;
	static float gotFloat;

	vm.addFunc("passString", std::function<void(const std::string&)>([&](const std::string& val){
		gotString = val;
	}));

	vm.addFunc("passNumber", std::function<void(int)>([&](int val){
		gotNumber = val;
	}));

	vm.addFunc("passFloat", std::function<void(const float&)>([&](const float& val){
		gotFloat = val;
	}));

	const std::string src =
		"::passString(MyClass.MyConstString);"
		"::passNumber(MyClass.MyConstNumber);"
		"::passFloat(MyClass.MyConstFloat);";

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(gotString == "Hello World");
	REQUIRE(gotNumber == 42);
	REQUIRE(gotFloat == Approx(123.456f));
}

TEST_CASE("Bind class with method of void pointer argument") {
	static void* specialPointer = NULL;
	static void* randomPointer = (void*)0x123456ULL;
	static const void* randomPointerSet = NULL;

	class MyClass {
	public:
		MyClass() {
			
		}
		~MyClass() {
			
		}
		void setPtr(void* ptr) {
			specialPointer = ptr;
		}
		const void* getPtr() const {
			return specialPointer;
		}
	};

	ssq::Machine vm(1024);
	ssq::Class<MyClass> cls = vm.addClass<MyClass>("MyClass");
	cls.addFunc("setPtr", std::function<void(MyClass*, void*)>( std::mem_fn(&MyClass::setPtr) ));
	cls.addFunc("getPtr", std::function<const void*(MyClass*)>( std::mem_fn(&MyClass::getPtr) ));

	const std::string src =
		"local myClass = MyClass();"
		"myClass.setPtr(getRandomPointer());"
		"setRandomPointer(myClass.getPtr());";

	vm.addFunc("getRandomPointer", std::function<void*(void)>([&](){
		return randomPointer;
	}));

	vm.addFunc("setRandomPointer", std::function<void(const void*)>([&](const void* val){
		randomPointerSet = val;
	}));

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(specialPointer == randomPointer);
	REQUIRE(randomPointerSet == randomPointer);
}

template<class T>
void validateResults(T& passByCopyResult, T& passByConstRefResult, T& getByCopyResult, T&getByConstRefResult, T& val) {
	CHECK(passByCopyResult == val);
	CHECK(passByConstRefResult == val);
	CHECK(getByCopyResult == val);
	CHECK(getByConstRefResult == val);
}

template<>
void validateResults<float>(float& passByCopyResult, float& passByConstRefResult, float& getByCopyResult, float&getByConstRefResult, float& val) {
	CHECK(passByCopyResult == Approx(val));
	CHECK(passByConstRefResult == Approx(val));
	CHECK(getByCopyResult == Approx(val));
	CHECK(getByConstRefResult == Approx(val));
}

template<>
void validateResults<double>(double& passByCopyResult, double& passByConstRefResult, double& getByCopyResult, double&getByConstRefResult, double& val) {
	CHECK(passByCopyResult == Approx(val));
	CHECK(passByConstRefResult == Approx(val));
	CHECK(getByCopyResult == Approx(val));
	CHECK(getByConstRefResult == Approx(val));
}

template<class T>
void passAndGet(T val, ssq::Machine& vm) {
	static T passByCopyResult;
	static T passByConstRefResult;
	static T getByCopyResult;
	static T getByConstRefResult;

	class MyClass {
	public:
		MyClass() {
			
		}
		~MyClass() {
			
		}
		void passByCopy(T val) {
			passByCopyResult = val;
			myVal = val;
		}
		void passByConstRef(const T& val) {
			passByConstRefResult = val;
			myVal = val;
		}
		T getByCopy() {
			return myVal;
		}
		const T& getByConstRef() {
			return myVal;
		}
		T myVal;
	};

	ssq::Class<MyClass> cls = vm.addClass<MyClass>("MyClass");

	cls.addFunc("passByCopy", std::function<void(MyClass*, T)>( std::mem_fn(&MyClass::passByCopy) ));
	cls.addFunc("passByConstRef", std::function<void(MyClass*, const T&)>( std::mem_fn(&MyClass::passByConstRef) ));

	cls.addFunc("getByCopy", std::function<T(MyClass*)>( std::mem_fn(&MyClass::getByCopy) ));
	cls.addFunc("getByConstRef", std::function<const T&(MyClass*)>( std::mem_fn(&MyClass::getByConstRef) ));

	vm.addFunc("getValue", std::function<T(void)>([&](){
		return val;
	}));

	vm.addFunc("setByCopyResult", std::function<void(T)>([&](T v){
		getByCopyResult = v;
	}));

	vm.addFunc("setByConstRefResult", std::function<void(const T&)>([&](const T& v){
		getByConstRefResult = v;
	}));

	static const std::string src =
		"local myClass = MyClass();"
		"local val = ::getValue();"
		"myClass.passByCopy(val);"
		"myClass.passByConstRef(val);"
		"::setByCopyResult(myClass.getByCopy());"
		"::setByConstRefResult(myClass.getByConstRef());";

	ssq::Script script = vm.compileFromData(src);
	vm.run(script);

	validateResults(passByCopyResult, passByConstRefResult, getByCopyResult, getByConstRefResult, val);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref int8_t") {
	ssq::Machine vm(1024);
	passAndGet<int8_t>(0x76, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref int16_t") {
	ssq::Machine vm(1024);
	passAndGet<int16_t>(0x7654, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref int32_t") {
	ssq::Machine vm(1024);
	passAndGet<int32_t>(0x76543210, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref int64_t") {
	ssq::Machine vm(1024);
	passAndGet<int64_t>(0x76543210, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref uint8_t") {
	ssq::Machine vm(1024);
	passAndGet<uint8_t>(0xF1, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref uint16_t") {
	ssq::Machine vm(1024);
	passAndGet<uint16_t>(0xF123, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref uint32_t") {
	ssq::Machine vm(1024);
	passAndGet<uint32_t>(0xF1234567, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref uint64_t") {
	ssq::Machine vm(1024);
	passAndGet<uint64_t>(0xF1234567, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref float") {
	ssq::Machine vm(1024);
	passAndGet<float>(123.456f, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref double") {
	ssq::Machine vm(1024);
	passAndGet<double>(123.456, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref bool") {
	ssq::Machine vm(1024);
	passAndGet<bool>(true, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref std::string") {
	ssq::Machine vm(1024);
	passAndGet<std::string>("Hello World", vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref void*") {
	ssq::Machine vm(1024);
	passAndGet<void*>((void*)0x123456ULL, vm);
}

TEST_CASE("Test pass by copy/ref & get by copy/ref custom class") {
	class Foo {
	public:
		Foo() {
			
		}
		~Foo() {
			
		}
	};
	
	ssq::Machine vm(1024);
	vm.addClass<Foo>("Foo");

	std::unique_ptr<Foo> foo(new Foo());
	passAndGet<Foo*>(foo.get(), vm);
}

TEST_CASE("Test return by pointer and call member function") {
	static std::string strSet;

	class Foo {
	public:
		Foo() {
			
		}
		~Foo() {
			
		}
		void callMe(const std::string& str) {
			strSet = str;
			bar = str;
		}
		std::string bar;
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo");
	cls.addFunc("callMe", std::function<void(Foo*, const std::string&)>( std::mem_fn(&Foo::callMe) ));

	std::unique_ptr<Foo> foo(new Foo());
	vm.addFunc("getFooPtr", std::function<Foo*(void)>([&](){
		return foo.get();
	}));

	static const std::string src =
		"local foo = getFooPtr();"
		"foo.callMe(\"Hello World\");";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(strSet == "Hello World");
}

TEST_CASE("Test return by const reference and call member function") {
	static std::string strSet;

	class Bar {
	public:
		int val;
	};

	static Bar barSet;

	class Foo {
	public:
		Foo() {
			bar.val = 123456;
		}
		~Foo() {
			
		}
		void callMe(const std::string& str) {
			strSet = str;
			baz = str;
		}

		const Bar& getBar() const {
			return bar;
		}

		void callBar(const Bar& b) {
			barSet = b;
		}
		std::string baz;
		Bar bar;
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo");
	vm.addClass<Bar>("Bar");
	cls.addFunc("callMe", std::function<void(Foo*, const std::string&)>( std::mem_fn(&Foo::callMe) ));
	cls.addFunc("getBar", std::function<const Bar&(Foo*)>( std::mem_fn(&Foo::getBar) ));
	cls.addFunc("callBar", std::function<void(Foo*, const Bar&)>( std::mem_fn(&Foo::callBar) ));

	std::unique_ptr<Foo> foo(new Foo());

	vm.addFunc("getFooPtr", std::function<Foo*(void)>([&](){
		return foo.get();
	}));

	static const std::string src =
		"local foo = getFooPtr();"
		"local bar = foo.getBar();"
		"foo.callBar(bar);"
		"foo.callMe(\"Hello World\");";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(strSet == "Hello World");
	REQUIRE(barSet.val == 123456);
}

TEST_CASE("Default constructor") {
	class Foo;
	static Foo* fooAllocatedPtr = nullptr;

	class Foo {
	public:
		Foo() {
			fooAllocatedPtr = this;
		}
		~Foo() {
		}
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo", std::function<Foo*()>([&](){
		return new Foo();
	}));

	static Foo* fooPtrCheck = nullptr;
	vm.addFunc("passFoo", std::function<void(Foo*)>([&](Foo* foo){
		fooPtrCheck = foo;
	}));

	static const std::string src =
		"local foo = Foo();"
		"passFoo(foo);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(fooPtrCheck != nullptr);
	REQUIRE(fooPtrCheck == fooAllocatedPtr);
}

TEST_CASE("Non default constructor") {
	static int posxSet;
	static int posySet;
	class Foo;
	static Foo* fooAllocatedPtr = nullptr;

	class Foo {
	public:
		Foo(int x, int y):posx(x),posy(y) {
			fooAllocatedPtr = this;
			posxSet = x;
			posySet = y;
		}
		~Foo() {
			
		}

	private:
		int posx;
		int posy;
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo", std::function<Foo*(int, int)>([&](int x, int y){
		return new Foo(x, y);
	}));

	static Foo* fooPtrCheck = nullptr;
	vm.addFunc("passFoo", std::function<void(Foo*)>([&](Foo* foo){
		fooPtrCheck = foo;
	}));

	static const std::string src =
		"local foo = Foo(42, 64);"
		"passFoo(foo);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(posxSet == 42);
	REQUIRE(posySet == 64);
	REQUIRE(fooPtrCheck != nullptr);
	REQUIRE(fooPtrCheck == fooAllocatedPtr);
}

TEST_CASE("Non default constructor via consturctor deduction helper") {
	static int posxSet;
	static int posySet;
	class Foo;
	static Foo* fooAllocatedPtr = nullptr;

	class Foo {
	public:
		Foo(int x, int y):posx(x),posy(y) {
			fooAllocatedPtr = this;
			posxSet = x;
			posySet = y;
		}
		~Foo() {
			
		}

	private:
		int posx;
		int posy;
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo", ssq::Constructor<Foo(int, int)>());

	static Foo* fooPtrCheck = nullptr;
	vm.addFunc("passFoo", std::function<void(Foo*)>([&](Foo* foo){
		fooPtrCheck = foo;
	}));

	static const std::string src =
		"local foo = Foo(42, 64);"
		"passFoo(foo);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(posxSet == 42);
	REQUIRE(posySet == 64);
	REQUIRE(fooPtrCheck != nullptr);
	REQUIRE(fooPtrCheck == fooAllocatedPtr);
}

TEST_CASE("Non default partial constructor") {
	static int posxSet;
	static int posySet;
	class Foo;
	static Foo* fooAllocatedPtr = nullptr;

	class Foo {
	public:
		Foo(int x, int y):posx(x),posy(y) {
			fooAllocatedPtr = this;
			posxSet = x;
			posySet = y;
		}
		~Foo() {
			
		}

	private:
		int posx;
		int posy;
	};

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo", std::function<Foo*(int)>([&](int y){
		return new Foo(42, y);
	}));

	static Foo* fooPtrCheck = nullptr;
	vm.addFunc("passFoo", std::function<void(Foo*)>([&](Foo* foo){
		fooPtrCheck = foo;
	}));

	static const std::string src =
		"local foo = Foo(64);"
		"passFoo(foo);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(posxSet == 42);
	REQUIRE(posySet == 64);
	REQUIRE(fooPtrCheck != nullptr);
	REQUIRE(fooPtrCheck == fooAllocatedPtr);
}

TEST_CASE("Non default constructor with class pointer") {
	static bool guiDestructor = false;
	
	class Gui {
	public:
		Gui() {
			
		}
		~Gui() {
			guiDestructor = true;
		}
	};

	static Gui* constructorGuiSet = nullptr;
	static std::string constructorLabelSet;

	class Widget {
	public:
		Widget(Gui* gui, const std::string label) {
			constructorGuiSet = gui;
			constructorLabelSet = label;
		}
		~Widget() {
			
		}
	};

	ssq::Machine vm(1024);
	vm.addClass<Gui>("Gui");

	auto cls = vm.addClass<Widget>("Widget", std::function<Widget*(Gui*, const std::string&)>([&](Gui* gui, const std::string& label){
		return new Widget(gui, label);
	}));

	std::unique_ptr<Gui> gui(new Gui());
	vm.addFunc("getGui", std::function<Gui*()>([&](){
		return gui.get();
	}));

	static const std::string src =
		"local gui = getGui();"
		"local widget = Widget(gui, \"Label\");";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(guiDestructor == false);
	REQUIRE(constructorGuiSet == gui.get());
	REQUIRE(constructorLabelSet == "Label");
}

template<class T>
void callGlobalFunc(T val, ssq::Machine& vm) {
	
}

TEST_CASE("Get squirrel class and find squirrel member function") {
	static const std::string src =
		"class Vec2 {"
		"    constructor(x_, y_){"
		"        x = x_;"
		"        y = y_;"
		"    }"
		"    function set(x_, y_){"
		"        x = x_;"
		"        y = y_;"
		"    }"
		"    x = 0;"
		"    y = 0;"
		"}"
		"local vec = Vec2(10, 20);"
		"::passClassConstRef(Vec2);"
		"::passClass(Vec2);";

	ssq::Machine vm(1024);
	auto script = vm.compileFromData(src);

	std::unique_ptr<ssq::Function> funcSetConstRef;
	std::unique_ptr<ssq::Function> funcSet;

	//vm.findClass("Vec2");
	vm.addFunc("passClassConstRef", std::function<void(const ssq::RawClass&)>([&](const ssq::RawClass& cls){
		auto func = cls.findFunc("set");
		funcSetConstRef.reset(new ssq::Function(std::move(func)));
	}));

	//vm.findClass("Vec2");
	vm.addFunc("passClass", std::function<void(ssq::RawClass)>([&](ssq::RawClass cls){
		auto func = cls.findFunc("set");
		funcSet.reset(new ssq::Function(std::move(func)));
	}));

	vm.run(script);
	REQUIRE(funcSetConstRef->isEmpty() == false);
	REQUIRE(funcSet->isEmpty() == false);
}

TEST_CASE("Get squirrel class instance") {
	static const std::string src =
		"class Vec2 {"
		"    constructor(x_, y_){"
		"        x = x_;"
		"        y = y_;"
		"    }"
		"    function set(x_, y_){"
		"        x = x_;"
		"        y = y_;"
		"    }"
		"    function getX(){"
		"        return x;"
	    "    }"
	    "    function getY(){"
		"        return y;"
	    "    }"
		"    x = 0;"
		"    y = 0;"
		"}"
		"local vec = Vec2(10, 20);"
		"::passInstanceConstRef(vec);"
		"::passInstance(vec);";

	ssq::Machine vm(1024);
	auto script = vm.compileFromData(src);

	std::unique_ptr<ssq::Instance> instSetConstRef;
	std::unique_ptr<ssq::Instance> instSet;

	//vm.findClass("Vec2");
	vm.addFunc("passInstanceConstRef", std::function<void(const ssq::Instance&)>([&](const ssq::Instance& inst){
		//instSetConstRef.reset(new ssq::Instance(std::move(cls)));
		REQUIRE(inst.isEmpty() == false);
		ssq::RawClass cls = inst.getClass();
		REQUIRE(cls.isEmpty() == false);

		auto funcSet = cls.findFunc("set");
		vm.callMember<void>(inst, funcSet, 42, 64);

		auto funcGetX = cls.findFunc("getX");
		auto funcGetY = cls.findFunc("getY");

		auto x = vm.callMember<int>(inst, funcGetX);
		auto y = vm.callMember<int>(inst, funcGetY);

		REQUIRE(x == 42);
		REQUIRE(y == 64);
	}));

	//vm.findClass("Vec2");
	vm.addFunc("passInstance", std::function<void(ssq::Instance)>([&](ssq::Instance inst){
		//instSet.reset(new ssq::Instance(std::move(cls)));
		REQUIRE(inst.isEmpty() == false);
		ssq::RawClass cls = inst.getClass();
		REQUIRE(cls.isEmpty() == false);

		auto funcSet = cls.findFunc("set");
		vm.callMember<void>(inst, funcSet, 32, 96);

		auto funcGetX = cls.findFunc("getX");
		auto funcGetY = cls.findFunc("getY");

		auto x = vm.callMember<int>(inst, funcGetX);
		auto y = vm.callMember<int>(inst, funcGetY);

		REQUIRE(x == 32);
		REQUIRE(y == 96);
	}));

	vm.run(script);
	//REQUIRE(funcSetConstRef->isEmpty() == false);
	//REQUIRE(funcSet->isEmpty() == false);
}

TEST_CASE("Register class but except different one from squirrel") {
	class Foo {
	public:
		Foo() {
			
		}
		~Foo() {
			
		}
	};

	class Bar {
	public:
		Bar() {
			
		}
		~Bar() {
			
		}
	};

	ssq::Machine vm(1024);
	vm.addClass<Foo>("Foo");
	vm.addClass<Bar>("Bar");

	vm.addFunc("passFoo", std::function<void(Foo*)>([&](Foo* foo){
		FAIL("The passFoo function must never get called!");
	}));

	static const std::string src =
		"local bar = Bar();"
		"::passFoo(bar);"; // Pass different class on purpose

	auto script = vm.compileFromData(src);
	REQUIRE_THROWS_AS(
        vm.run(script), 
        ssq::RuntimeException
    );
}

TEST_CASE("Register custom function and throw C++ exception") {
	ssq::Machine vm(1024);

	vm.addFunc("doSomething", std::function<void(int)>([&](int val){
		(void)val;
		throw std::out_of_range("Out of range!");
	}));

	static const std::string src =
		"::doSomething(42);";

	auto script = vm.compileFromData(src);
	REQUIRE_THROWS_AS(
        vm.run(script), 
        ssq::RuntimeException
    );
}

TEST_CASE("Register custom function and throw C++ exception and catch it") {
	ssq::Machine vm(1024);

	vm.addFunc("doSomething", std::function<void(int)>([&](int val){
		(void)val;
		throw std::out_of_range("Out of range!");
	}));

	static const std::string src =
		"try {"
		"    ::doSomething(42);"
		"} catch (e) {"
		"}";

	auto script = vm.compileFromData(src);
	vm.run(script);
}

TEST_CASE("Register custom class with variable") {
	class Vec2 {
	public:
		Vec2(int x_, int y_) {
			x = x_;
			y = y_;
		}
		~Vec2() {
			
		}

		int x;
		int y;
	};

	Vec2 vec(10, 20);

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Vec2>("Vec2", ssq::Constructor<Vec2(int, int)>());

	cls.addVar("x", &Vec2::x);

	// Test if swap is working properly
	auto movedCls = std::move(cls);
	movedCls.addVar("y", &Vec2::y);

	vm.addFunc("getVec2", std::function<Vec2*()>([&](){
		return &vec;
	}));

	static const std::string src =
		"local vec = ::getVec2();"
		"vec.x = 42;"
		"vec.y = 65;";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(vec.x == 42);
	REQUIRE(vec.y == 65);
}

TEST_CASE("Register custom class with variable #2") {
	class Vec2 {
	public:
		Vec2(int x_, int y_) {
			x = x_;
			y = y_;
		}
		~Vec2() {
			
		}

		int x;
		int y;
	};

	static Vec2 vec2Set(0, 0);

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Vec2>("Vec2", ssq::Constructor<Vec2(int, int)>());

	cls.addVar("x", &Vec2::x);

	// Test if swap is working properly
	auto movedCls = std::move(cls);
	movedCls.addVar("y", &Vec2::y);

	vm.addFunc("setVec2", std::function<void(int, int)>([&](int x, int y){
		vec2Set.x = x;
		vec2Set.y = y;
	}));

	static const std::string src =
		"local vec = Vec2(10, 20);"
		"vec.x = 42;"
		"vec.y = 65;"
		"setVec2(vec.x, vec.y);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(vec2Set.x == 42);
	REQUIRE(vec2Set.y == 65);
}

TEST_CASE("Pass table to squirrel") {
	static std::string stringSet;
	static int integerSet;
	static float floatSet;

	ssq::Machine vm(1024);

	ssq::Table table = vm.newTable();
	table.addSlot("string", "world");
	table.addSlot("integer", 42);
	table.addSlot("float", 123.456f);

	vm.addFunc("setString", std::function<void(const std::string&)>([&](const std::string& val){
		stringSet = val;
	}));

	vm.addFunc("setInteger", std::function<void(int)>([&](int val){
		integerSet = val;
	}));

	vm.addFunc("setFloat", std::function<void(float)>([&](float val){
		floatSet = val;
	}));

	vm.addFunc("getTable", std::function<const ssq::Table&(void)>([&]() -> const ssq::Table& {
		return table;
	}));

	static const std::string src =
		"local table = ::getTable();"
		"::setString(table.string);"
		"::setInteger(table.integer);"
		"::setFloat(table.float);";

	auto script = vm.compileFromData(src);
	vm.run(script);
	
	REQUIRE(stringSet == "world");
	REQUIRE(integerSet == 42);
	REQUIRE(floatSet == Approx(123.456f));
}

TEST_CASE("Get table from squirrel") {
	ssq::Machine vm(1024);
	ssq::Table table = vm.newTable();
	int result;

	static const std::string src =
		"function add(a, b){ \n"
		"    return a + b;\n"
		"}\n"
		"local table = {\n"
		"    hello=\"Hello\",\n"
		"    integer=123,\n"
		"    func=add\n"
		"}\n"
		"::passTable(table);\n"
		"local ret = ::getTable();\n"
		"local result = ret.func(ret.integer, 100);\n"
		"::passResult(result);\n";

	vm.addFunc("passTable", std::function<void(ssq::Table)>([&](ssq::Table t){
		table = t;
		REQUIRE(t.isEmpty() == false);
		REQUIRE(table.isEmpty() == false);
	}));

	vm.addFunc("getTable", std::function<const ssq::Table&(void)>([&]() -> const ssq::Table& {
		return table;
	}));

	vm.addFunc("passResult", std::function<void(int)>([&](int res){
		result = res;
	}));

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(result == 223);
}

TEST_CASE("Pass array via iterator to squirrel") {
	ssq::Machine vm(1024);

	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(42);

	static int setLengthRes;
	static int setLastRes;

	vm.addFunc<ssq::Iterator<std::vector<int>::iterator>>("getArray", [&]() {
		return ssq::Iterator<std::vector<int>::iterator>(vec.begin(), vec.end());
	});

	vm.addFunc<void, int>("setLength",[&](int val){
		setLengthRes = val;
	});

	vm.addFunc<void, int>("setLast", [&](int val){
		setLastRes = val;
	});

	static const std::string src =
		"local array = ::getArray();"
		"::setLength(array.len());"
		"::setLast(array[2]);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(setLengthRes == 3);
	REQUIRE(setLastRes == 42);
}

TEST_CASE("Pass array via raw pointer to squirrel") {
	ssq::Machine vm(1024);

	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(42);

	static int setLengthRes;
	static int setLastRes;

	vm.addFunc<ssq::RawArray<int>>("getArray", [&]() {
		return ssq::RawArray<int>(&vec[0], vec.size());
	});

	vm.addFunc<void, int>("setLength",[&](int val){
		setLengthRes = val;
	});

	vm.addFunc<void, int>("setLast", [&](int val){
		setLastRes = val;
	});

	static const std::string src =
		"local array = ::getArray();"
		"::setLength(array.len());"
		"::setLast(array[2]);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(setLengthRes == 3);
	REQUIRE(setLastRes == 42);
}

TEST_CASE("Pass array via std::vector to squirrel") {
	ssq::Machine vm(1024);

	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(42);

	static int setLengthRes;
	static int setLastRes;

	vm.addFunc<std::vector<int>>("getArray", [&]() {
		return vec;
	});

	vm.addFunc<void, int>("setLength",[&](int val){
		setLengthRes = val;
	});

	vm.addFunc<void, int>("setLast", [&](int val){
		setLastRes = val;
	});

	static const std::string src =
		"local array = ::getArray();"
		"::setLength(array.len());"
		"::setLast(array[2]);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(setLengthRes == 3);
	REQUIRE(setLastRes == 42);
}

TEST_CASE("Pass array via const std::vector& to squirrel") {
	ssq::Machine vm(1024);

	static int setLengthRes;
	static int setLastRes;

	static std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(42);

	vm.addFunc("getArray", std::function<const std::vector<int>&(void)>([&]() -> const std::vector<int>& {
		return vec;
	}));

	vm.addFunc<void, int>("setLength",[&](int val){
		setLengthRes = val;
	});

	vm.addFunc<void, int>("setLast", [&](int val){
		setLastRes = val;
	});

	static const std::string src =
		"local array = ::getArray();"
		"::setLength(array.len());"
		"::setLast(array[2]);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(setLengthRes == 3);
	REQUIRE(setLastRes == 42);
}

TEST_CASE("Get array via std::vector from squirrel") {
	ssq::Machine vm(1024);

	static std::vector<int> arraySet;

	vm.addFunc("setArray", std::function<void(std::vector<int>)>([&](std::vector<int> vec)  {
		arraySet = vec;
	}));

	static const std::string src =
		"local array = [10, 20, 42];"
		"::setArray(array);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(arraySet.size() == 3);
	REQUIRE(arraySet[0] == 10);
	REQUIRE(arraySet[1] == 20);
	REQUIRE(arraySet[2] == 42);
}

TEST_CASE("Get array via const std::vector& from squirrel") {
	ssq::Machine vm(1024);

	static std::vector<int> arraySet;

	vm.addFunc("setArray", std::function<void(const std::vector<int>&)>([&](const std::vector<int>& vec)  {
		arraySet = vec;
	}));

	static const std::string src =
		"local array = [10, 20, 42];"
		"::setArray(array);";

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(arraySet.size() == 3);
	REQUIRE(arraySet[0] == 10);
	REQUIRE(arraySet[1] == 20);
	REQUIRE(arraySet[2] == 42);
}

TEST_CASE("Pass class by copy") {
	class Foo {
	public:
		Foo() {
			
		}
		~Foo() {
			
		}

		int getVal() {
			return val;
		}

		void setVal(int v) {
			val = v;
		}

		int val;
	};

	static Foo fooSet;

	ssq::Machine vm(1024);
	auto cls = vm.addClass<Foo>("Foo");
	cls.addFunc("getVal", std::function<int(Foo*)>( std::mem_fn(&Foo::getVal) ));
	cls.addFunc("setVal", std::function<void(Foo*, int)>( std::mem_fn(&Foo::setVal) ));

	static const std::string src =
		"local foo = ::getFoo();"
		"foo.setVal(42);"
		"::setFoo(foo);";

	vm.addFunc("getFoo", std::function<Foo(void)>([&](){
		return Foo();
	}));

	vm.addFunc("setFoo", std::function<void(Foo)>([&](Foo foo){
		fooSet = foo;
	}));

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(fooSet.getVal() == 42);
}

TEST_CASE("Move and copy") {
	ssq::Machine vm(1024);

	auto a = vm.newTable();
	auto b = a;
	auto c = std::move(a);

	REQUIRE(a.isEmpty() == true);
	REQUIRE(b.isEmpty() == false);
	REQUIRE(c.isEmpty() == false);

	a.destroy();
	b.destroy();
	c.destroy();

	a.destroy();
	b.destroy();
	c.destroy();

	a.destroy();
	b.destroy();
	c.destroy();

	auto d = vm.newTable();
}

TEST_CASE("Pass raw class and raw function and call it from C++") {
	ssq::Machine vm(1024);

	static const std::string src =
		"class Foo {\n"
		"    function bar(a, b, c){\n"
		"        val = (a + b) * c;\n"
		"    }\n"
		"    val = -1;\n"
		"};\n"
		"local foo = Foo();\n"
		"::passInstanceAndFunc(foo, Foo.bar);\n"
		"::doMagic();\n"
		"::passResult(foo.val);\n";

	ssq::Instance instance;
	ssq::Function function;
	int result = 0;

	vm.addFunc("passInstanceAndFunc", std::function<void(ssq::Instance, ssq::Function)>(
		[&](ssq::Instance inst, ssq::Function func){
			REQUIRE(inst.isEmpty() == false);
			REQUIRE(func.isEmpty() == false);
			instance = inst;
			function = func;
			REQUIRE(instance.isEmpty() == false);
			REQUIRE(function.isEmpty() == false);
		}
	));

	vm.addFunc("doMagic", std::function<void(void)>([&](){
		REQUIRE(instance.isEmpty() == false);
		REQUIRE(function.isEmpty() == false);

		vm.callMember<void>(instance, function, 5, 3, 10);
	}));

	vm.addFunc("passResult", std::function<void(int)>([&](int res){
		result = res;
	}));

	auto script = vm.compileFromData(src);
	vm.run(script);

	REQUIRE(result == (5 + 3) * 10);
}
