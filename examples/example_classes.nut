local instance = CppClass("Hello World!", 42);

print("Instance has message: " + instance.getMessage());
print("Lowercase version: " + instance.getMessageLowerCase());
print("Instance has integer: " + instance.getInteger());

instance.message = "Banana!";

function getInstance() {
    return instance;
}

function doStuff(ptr) {
    ptr.setMessage("This is a message from Squirrel!");
    instance = ptr;
}

function foo() {
    print("Instance now contains message: " + instance.message);
}
