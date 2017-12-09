local str = appendString("World");
print("appendString returned: " + str);

local time = getLocalTime();
print("Your local time is: " + time);

local result = doStuff(10, 123.456, true);
print("doStuff returned: " + result);

function foo(msg){
    print("Received message: " + msg);
}

function bar(a, b, c){
    return a + b + c;
}
