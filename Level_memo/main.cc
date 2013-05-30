#include "arena.h"
#include "memo.h"
#include <stdio.h>
#include <iostream>
using namespace std;

int main() {
    leveldb::Arena arena;
    cout << arena.MemoryUsage() << endl;

    size_t bytes = 1030;
    cout << "If the bytes >= 1M" << bytes << endl;
    //printf("0x%08x\n",arena.Allocate(bytes));
    printf("0x%08x\n",arena.AllocateAligned(bytes));

    bytes = 512;
    cout << "If the bytes <= 1M " << bytes << endl;
    printf("0x%08x\n",arena.Allocate(bytes));

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
    return 0;
}
