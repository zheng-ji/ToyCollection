#include <stdio.h>
#include <iostream>
#include "mystatus.h"
using namespace std;

int main() {

    Slice msg1("hello");
    Slice msg2("World");
    Status s = Status::NotFound(msg1,msg2);
    cout << s.ToString() << endl;

    return 0;
}
