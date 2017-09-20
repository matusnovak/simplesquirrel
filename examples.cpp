#define CATCH_CONFIG_MAIN 
#include <catch/catch.hpp>
#include "include/simplesq.hpp"

#define STRINGIFY(x) #x

std::string simpleGlobalFunc(float f) {
	return "The answer is: " + std::to_string(sqrtf(f));
}

TEST_CASE("Register simple C++ global function"){
    ssq::Machine vm(1024);
    
	static const std::string source = STRINGIFY(
		local f = 1024.0;
		local answer = simpleGlobalFunc(f);
		print("Returned: \"" + answer + "\"");
	);

	// Sometimes, due to c++ compiler templates,
	// you will be required to specify return type!
	vm.addFunc<std::string>("simpleGlobalFunc", &simpleGlobalFunc);

	auto script = vm.compileFromData(source);
	vm.run(script); 
}

TEST_CASE("Register simple C++ global function via lambda"){
    ssq::Machine vm(1024);
    
	static const std::string source = STRINGIFY(
		local f = 1024.0;
		local answer = simpleGlobalFunc(f);
		print("Returned: \"" + answer + "\"");
	);

	// Can be used to accept function pointer or std::function with lambda.
	// If you are using std::function, the return type is automatically
	// deduced. (you don't need to explicitly declare vm.addFunc<std::string>(...);
	vm.addFunc("simpleGlobalFunc", std::function<std::string(float)>([](float f){
		return "The answer is: " + std::to_string(sqrtf(f));
	}));

	auto script = vm.compileFromData(source);
	vm.run(script); 
}

TEST_CASE("Call squirrel global function") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		function squirrelGlobalFunc(a, b, c) {
			print("var a is type: " + (typeof a) + " and holds: " + a);
			print("var b is type: " + (typeof b) + " and holds: " + b[0] + ", " + b[1] + ", " + b[2]);
			print("var c is type: " + (typeof c) + " and holds: " + c);
			return "Hello World!";
		}
	);

	// First, compile and run the script!
	// The function won't exist until you run the script.
	// It's called dynamic language for a reason.
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// Then look up the function
	// If the function does not exist, ssq::NotFoundException is thrown
	// If "squirrelGlobalFunc" is not a funcion, ssq::TypeException is thrown
	ssq::Function func = vm.findFunc("squirrelGlobalFunc");

	// Notice that we have to specify what data type are we expecting 
	// from squirrel.
	// if something went wrong, an ssq::Excpetion is thrown
	vm.callGlobal<std::string>(func, 
		std::string("This is a string!"),  // a (has to be std::string and not c-string!)
		std::vector<int>({1, 5, 42}),      // b
		123.478f                           // c
	); 
}

class SimpleCppClass {
public:
	SimpleCppClass() {
		msg = "No message";
	}
	SimpleCppClass(const std::string& message) {
		msg = message;
	}
	~SimpleCppClass() {
		
	}
	const std::string& getMessage() const {
		return msg;
	}
	void setMessage(const std::string& message) {
		msg = message;
	}
	std::string msg;
};

TEST_CASE("Register simple C++ class") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		local instance = SimpleCppClass("This is a message!");

		print("The message is: \"" + instance.getMessage() + "\"");

		instance.setMessage("I am setting this from squirrel!");

		passClassInstance(instance);

		instance.msg = "Setting msg directly!";

		passClassInstance(instance);
	);

	// Register the class with all member functions.
	// Because the class has two constructors, we need to specify
	// which one to use.
	// Because the squirrel is constructing the class, the destructor
	// will be automatically called once the instance falls out of scope
	// within the script.
	ssq::Class<SimpleCppClass> myClass = vm.addClass<SimpleCppClass>(
		"SimpleCppClass",                                     // Name of the class
		ssq::Constructor<SimpleCppClass(std::string)>()  // Which constructor to use
	);

	// Now the member methods...
	// Due to compiler template issues, you have to manually
	// provide return type!
	myClass.addFunc<std::string>("getMessage", &SimpleCppClass::getMessage);
	myClass.addFunc<void>("setMessage", &SimpleCppClass::setMessage);

	// We can also bind attributes directly.
	// The last parameter says if the variable is static
	myClass.addVar("msg", &SimpleCppClass::msg, false);

	// This is a global function which will be used to get back the class
	vm.addFunc("passClassInstance", std::function<void(SimpleCppClass*)>([](SimpleCppClass* ptr){
		std::cout << "Printing out message from class instance: " << ptr->getMessage() << std::endl;
	}));

	auto script = vm.compileFromData(source);
	vm.run(script); 
}

class BaseClass {
public:
	virtual std::string getMessage() const = 0;
};

class DerivedClass: public BaseClass {
public:
	DerivedClass() {
		
	}
	~DerivedClass() {
		
	}
	std::string getMessage() const override {
		return "Hello World from DerivedClass";
	}
};

TEST_CASE("Register abstract C++ class") {
	std::unique_ptr<BaseClass> derived(new DerivedClass());

	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		function squirrelGlobalFunc(b) {
			print("base: " + b.getMessage());
		}
	);

	// Register our base class
	// No allocator (constructor) needed!
	// The squirrel will not construct to destruct the class!
	ssq::Class<BaseClass> klass = vm.addAbstractClass<BaseClass>("BaseClass");

	// Yes, you can bind pure virtual functions
	klass.addFunc<std::string>("getMessage", &BaseClass::getMessage);

	// First, compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// Then find the foo function
	ssq::Function squirrelGlobalFunc = vm.findFunc("squirrelGlobalFunc");

	// Call the global function
	vm.callGlobal<void>(squirrelGlobalFunc, derived.get());
}

class Widget {
public:
	enum Type {
		BUTTON,
		CHECKBOX,
		RADIO,
	};
	Widget(Type type, const std::string& label):type(type),label(label) {
		
	}
	virtual ~Widget() {
		
	}
	void setLabel(const std::string& str) {
		label = str;
	}
	const std::string& getLabel() const {
		return label;
	}
public:
	std::string label;
	Type type;
};

TEST_CASE("Squirrel class extending from C++ class") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		class Button extends Widget {
			constructor(label) {
				base.constructor(Widget.Type.BUTTON, label);
			}

			function appendToLabel(str) {
				setLabel(getLabel() + str);
			}
		};

		local button = Button("This is a button");
		print("Created a new button with label: " + button.getLabel());
		button.appendToLabel("_suffix");
		print("Button new label: " + button.getLabel());
	);

	std::unique_ptr<Widget> widget;

	// Register our class...
	// Because we use enums, we have to use std::function as our constructor
	// and then convert integer value into enumeration.
	// If you wish for your class to NOT to be deallocated inside of squirrel
	// (let's say you keep track of instances and do cleanup yourself)
	// Then you can provide third parameter into the addClass function
	// as 'false' (defaults to true) which disables release (deletion of instance).
	ssq::Class<Widget> klass = vm.addClass<Widget>("Widget", 
		std::function<Widget*(int, std::string)>([&](int type, std::string label){
		auto ptr = new Widget(Widget::Type(type), label);
		widget.reset(ptr); // Keep track of our instance
		// Normally the release is done by squirrel but this time
		// we want to do it manually.
		return ptr;
	}), false);

	klass.addFunc<void>("setLabel", &Widget::setLabel);
	klass.addFunc<const std::string&>("getLabel", &Widget::getLabel);

	ssq::Enum klassEnum = klass.addEnum("Type");
	klassEnum.addSlot("BUTTON", (int)Widget::Type::BUTTON);
	klassEnum.addSlot("CHECKBOX", (int)Widget::Type::CHECKBOX);
	klassEnum.addSlot("RADIO", (int)Widget::Type::RADIO);

	// Compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// The instance gets deleted here:
	widget.reset();
}

TEST_CASE("Getting class from squirrel") {
	ssq::Machine vm(1024);
	
	static const std::string source = STRINGIFY(
		class CustomClass {
			constructor(a, b, c) {
				msg = "Constructor: " + a + ", " + b + ", " + c;
				print(msg);
			}

			function setMessage(str) {
				msg = str;
			}

			function getMessage() {
				return msg;
			}

			msg = "Hello World!";
		};

		
	);

	// First, compile and run the script!
	// The class won't exist until you run the script.
	// It's called dynamic language for a reason.
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// Find the class
	ssq::RawClass customClass = vm.findClass("CustomClass");

	// Then create an instance of it
	ssq::Instance classInstance = vm.instanceFromClass(customClass);

	// Find the constructor
	ssq::Function constructor = customClass.findFunc("constructor");

	// Check number of args
	std::cout << "The CustomClass constructor accepts " << constructor.getNumOfParams() << " parameters" << std::endl;
	
	// Call the constructor 
	vm.callMember<void>(classInstance, constructor, std::string("Hello World!"), 123, true);

	// Get another method and call it
	ssq::Function setMessage = customClass.findFunc("setMessage");
	vm.callMember<void>(classInstance, setMessage, std::string("New Message from C++!"));

	// Get another method and call it
	ssq::Function getMessage = customClass.findFunc("getMessage");
	std::string msg = vm.callMember<std::string>(classInstance, getMessage);
	std::cout << "Got message back from squirrel: " << msg << std::endl;
}

class Foo {
public:
	enum Bar {
		HELLO = 42,
	};
};

TEST_CASE("Register enums and constants") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		function squirrelGlobalFunc() {
			print("Foo.BAR.HELLO constants is: " + Foo.Bar.HELLO);
			print("Foo.BAR.WORLD constants is: " + Foo.Bar.WORLD);
		}
	);

	// Register our class
	ssq::Class<Foo> klass = vm.addAbstractClass<Foo>("Foo");

	// Add enumeration
	ssq::Enum bar = klass.addEnum("Bar");
	bar.addSlot("HELLO", (int)Foo::Bar::HELLO);
	bar.addSlot("WORLD", std::string("World!"));

	// First, compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// Then find the foo function
	ssq::Function squirrelGlobalFunc = vm.findFunc("squirrelGlobalFunc");

	// Call the global function
	vm.callGlobal<void>(squirrelGlobalFunc);
}


TEST_CASE("Call C++ function and throw exception") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY( 
		function squirrelGlobalFunc() {
			cppGlobalFunc();
		}
	);

	// Register out function that throws
	vm.addFunc<void>("cppGlobalFunc", std::function<void(void)>([](){
		throw std::runtime_error("Some random exception in C++");
	}));

	// First, compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 

	// Then find the foo function
	ssq::Function squirrelGlobalFunc = vm.findFunc("squirrelGlobalFunc");

	// Call the global function
	try {
		vm.callGlobal<void>(squirrelGlobalFunc);
	} catch (ssq::RuntimeException& e) {
		std::cout << "We have detected exception while calling squirrelGlobalFunc" << std::endl;
		std::cout << "which is: " << e.what() << std::endl;
	}
}

TEST_CASE("Call C++ function and get array from iterative container") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		local arr = cppGlobalFunc();
		for(local i = 0; i < arr.len(); i++) {
			print("Got array from C++ index: " + i + " = " + arr[i]);
		}
	);

	std::list<int> list = { 10, 20, 30, 40, 50, 60 };

	// Register out function that returns an array.
	// The array is actually a list.
	// We can't pass list directly, but we can pass an iterator.
	// Note that the type of iterator has to be registered with squirrel VM,
	// or a simple plain data type (int, float, std::string, etc...)
	vm.addFunc("cppGlobalFunc", std::function<ssq::Iterator<std::list<int>::iterator>(void)>([&](){
		return ssq::Iterator<std::list<int>::iterator>(list.begin(), list.end());
	}));

	// Compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 
}

TEST_CASE("Call C++ function and get array from raw C array") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		local arr = cppGlobalFunc();
		for(local i = 0; i < arr.len(); i++) {
			print("Got array from C++ index: " + i + " = " + arr[i]);
		}
	);

	std::unique_ptr<int> arr(new int[6]);
	arr.get()[0] = 10;
	arr.get()[1] = 20;
	arr.get()[2] = 30;
	arr.get()[3] = 40;
	arr.get()[4] = 50;
	arr.get()[5] = 60;

	// Register out function that returns an array.
	// The array is actually a list.
	// We can't pass list directly, but we can pass an iterator.
	// Note that the type of iterator has to be registered with squirrel VM,
	// or a simple plain data type (int, float, std::string, etc...)
	vm.addFunc("cppGlobalFunc", std::function<ssq::RawArray<int>(void)>([&](){
		return ssq::RawArray<int>(arr.get(), 6);
	}));

	// Compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 
}

TEST_CASE("Call C++ function and get array from std::vector") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		local arr = cppGlobalFunc();
		for(local i = 0; i < arr.len(); i++) {
			print("Got array from C++ index: " + i + " = " + arr[i]);
		}
	);

	std::vector<int> vec = { 10, 20, 30, 40, 50, 60 };

	// Register out function that returns an array.
	// The array is actually a list.
	// We can't pass list directly, but we can pass an iterator.
	// Note that the type of iterator has to be registered with squirrel VM,
	// or a simple plain data type (int, float, std::string, etc...)
	vm.addFunc("cppGlobalFunc", std::function<std::vector<int>(void)>([&](){
		return vec;
	}));

	// Compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 
}

TEST_CASE("Get array from squirrel") {
	ssq::Machine vm(1024);

	static const std::string source = STRINGIFY(
		local arr = array(6, 0);
		arr[0] = 10;
		arr[1] = 20;
		arr[2] = 30;
		arr[3] = 40;
		arr[4] = 50;
		arr[5] = 60;
		cppGlobalFunc(arr);
	);

	// At this time, arrays passed from squirrel must contain only one data type.
	// No mixed typed allowed.
	vm.addFunc("cppGlobalFunc", std::function<void(std::vector<int>)>([&](std::vector<int> vec){
		std::cout << "got the following array from squirrel: ";
		for(const auto& e : vec) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}));

	// Compile and run
	auto script = vm.compileFromData(source);
	vm.run(script); 
}
