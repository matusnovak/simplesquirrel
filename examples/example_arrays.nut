local arr = [ 1234, "Test" ];

function set(val) {
    arr = val;
}

function get() {
    return arr;
}

function printContents(){
    for(local i = 0; i < arr.len(); i++){

    }
    print("index 0: " + arr[0]);
    print("index 1: " + arr[1]);
    print("index 2: " + arr[2]);
}
