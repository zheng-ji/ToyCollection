#include <stdio.h>
#include <iostream>
#include "myslice.h"
using namespace std;

int main() {


    /*
    CMemo memo;
    cout << memo.MemoryUsage() << endl;
    size_t bytes = 1030;
    cout << "If the bytes >= 1M" << bytes << endl;
    printf("0x%08x\n",memo.Allocate(bytes));
    
    bytes = 512;
    cout << "If the bytes <= 1M " << bytes << endl;
    printf("0x%08x\n",memo.Allocate(bytes));
    */
    Slice s1 = "hello world";
    Slice s2 = "hello world";
    Slice s3 = "hello";
    cout << s1.size() << endl;

    cout << "==============test remove_prefix() ==============" << endl;
    s1.remove_prefix(4);
    cout << s1.data() << endl;
    
    cout << "==============test operator ==  ==============" << endl;
    if (s1 == s2) {
        cout << "slice 1 and slice 2 are the same" << endl;
    } else {
        cout << s1.size() << " " << s2.size() << endl;
        cout << memcmp(s1.data(),s2.data(),s1.size()) << endl;
        cout << "no the same" << endl;
    }

    cout << "============test compare function ============" << endl;
    cout << s1.compare(s3) << endl;

    cout << "============test start with ==============" << endl;
    if (s2.starts_with(s3))
        cout << "Yes" << endl;
    return 0;
}
