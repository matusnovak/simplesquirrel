local dict = {
    test = 1234
    hello = "World!"
};
function set(val) {
    dict = val;
}
function get() {
    return dict;
}
function printContents(){
    print("test: " + dict.test);
    print("hello: " + dict.hello);
    print("banana: " + dict.banana);
}
