#include <stdio.h>
#include "MinHeap.h"

void findKMaxElementTest(int k)
{
    int a[10] = {20, 10, 11, 21, 36, 90, 100, 200, 25, 9};

    MinPriorityQueue pqueue = buildHeap_insert(a, 10);
    for (int i = 1; i < k; i++)
        deleteMin(pqueue);

    printf("第%d大元素为：%d\n", k, findMin(pqueue));
}

int main(int argc, const char * argv[])
{
    findKMaxElementTest(5);

    return 0;
}
