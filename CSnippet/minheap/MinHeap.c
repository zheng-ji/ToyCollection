#include <stdio.h>
#include <stdlib.h>
#include "MinHeap.h"

/* 标记节点，类似于链表中的表头节点
 * 该值必须小于所有最小堆中的元素，设其值为-1
 */
#define SentinelElement -1

/*
 * 使用数组实现堆
 *
 * capacity 数组的最大容量
 * size     数组的长度
 * elements 堆中的元素存放的数组
 */
struct MinHeap
{
    int capacity;
    int size;
    ElementType *elements; // 堆的元素个数为size，实际上用来存储的数组的长度为size + 1，还包括一个sentinel元素
};

void
PQueueNULLWarning()
{
    printf("Warning: Minimum Priority Queue is NULL");
}

void
outOfSpaceFatalError()
{
    printf("Fatal Error: Out of space");
    abort();
}

MinPriorityQueue
initialize(int maxElements)
{
    MinPriorityQueue pqueue;
    
    if (maxElements <= 0)
    {
        printf("Fail to initialize: maxElements <= 0");
        return NULL;
    }
    
    pqueue = malloc(sizeof(struct MinHeap));
    if (pqueue == NULL)
        outOfSpaceFatalError();
    
    // 数组的第0个元素是个sentinel标记节点，计入数组容量中，但不计入capcaity或size中
    pqueue->size = 0;
    pqueue->capacity = maxElements;
    pqueue->elements = malloc(sizeof(ElementType) * (pqueue->capacity + 1));
    if (pqueue->elements == NULL)
        outOfSpaceFatalError();
    else
        pqueue->elements[0] = SentinelElement;
    
    return pqueue;
}

void
destroy(MinPriorityQueue pqueue)
{
    if (pqueue != NULL)
    {
        // 在GNU99标准中，free(NULL)什么都不做直接返回，所以不用判断pqueue->elements是否为NULL
        free(pqueue->elements);
        free(pqueue);
    }
}

void
makeEmpty(MinPriorityQueue pqueue)
{
    if (pqueue != NULL)
        pqueue->size = 0;
    else
        PQueueNULLWarning();
}

/*
 * 插入时，堆中的元素执行下滤操作
 * 删除时，堆中的元素执行上滤操作
 */

/*
 * 插入的时间复杂度为O(log N)，N为最小堆中的元素个数
 * 实际上，其平均执行时间为O(1)
 */
void
insert(ElementType x, MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
        PQueueNULLWarning();
    
    if (isFull(pqueue))
    {
        printf("Fail to insert: Priority Queue is Full");
        return;
    }
    else
    {
        int i;
        
        // sentinel element在这里作为elements[0]被比较，是循环的终止条件
        for (i = ++pqueue->size; x < pqueue->elements[i / 2]; i /= 2)
            pqueue->elements[i] = pqueue->elements[i / 2]; // 下滤操作
        pqueue->elements[i] = x;
    }
}

/*
 * 删除操作的平均时间为O(log N)
 */
ElementType
deleteMin(MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
    {
        PQueueNULLWarning();
        return SentinelElement;
    }
    
    if (isEmpty(pqueue))
    {
        printf("Fail to delete: Priority Queue is Empty");
        return SentinelElement;
    }
    
    int i, child;
    ElementType minElement, lastElement;
    
    // 注意对某个节点进行上滤操作时，要判断该节点是有两个儿子还是一个儿子
    minElement = pqueue->elements[1];
    lastElement = pqueue->elements[pqueue->size--];
    for (i = 1; i * 2 <= pqueue->size; i = child)
    {
        child = i * 2;
        
        // 节点i只有一个儿子时必有i * 2 = pqueue->size
        if (child < pqueue->size && pqueue->elements[child] > pqueue->elements[child + 1])
            child++;
        
        if (lastElement < pqueue->elements[child])
            break;
        else
            pqueue->elements[i] = pqueue->elements[child]; // 上滤操作
    }
    pqueue->elements[i] = lastElement;
    
    return minElement; // 返回被删除的元素
}

/*
 * 执行时间：O(1)
 */
ElementType
findMin(MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
    {
        PQueueNULLWarning();
        return SentinelElement;
    }
    else
        return pqueue->elements[1];
}

int
isEmpty(MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
    {
        PQueueNULLWarning();
        return -1;
    }
    else
        return (pqueue->size == 0);
}

int
isFull(MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
    {
        PQueueNULLWarning();
        return -1;
    }
    else
        return (pqueue->size == pqueue->capacity);
}

void
percolateDown(int *arr, int len, int i)
{
    int child;
    int n = len - 1;
    ElementType tmp;
    
    for (tmp = arr[i]; i * 2 < n; i = child)
    {
        child = i * 2;
        if (child < n && arr[child] > arr[child + 1])
            child++;
        
        if (tmp > arr[child])
            arr[i] = arr[child];
        else
            break;
    }
    arr[i] = tmp;
}

MinPriorityQueue
buildHeap_percolate(int *arr, int n)
{
    if (arr == NULL)
    {
        printf("Error: Array is NULL");
        return NULL;
    }
    
    MinPriorityQueue pqueue;
    pqueue = malloc(sizeof(struct MinHeap));
    
    if (pqueue == NULL)
        outOfSpaceFatalError();
    ElementType *elements = malloc(sizeof(ElementType) * (n + 1));
    if (elements == NULL)
        outOfSpaceFatalError();
    
    int i;
    for (i = 1; i <= n; i++)
        elements[i] = arr[i - 1];
    elements[0] = SentinelElement;
    
    for (i = n / 2; i > 0; i--)
        percolateDown(elements, n + 1, i);
    
    pqueue->elements = elements;
    pqueue->size = n;
    pqueue->capacity = n * 2;
    
    return pqueue;
}

/*
 * 通过n次插入元素建立堆，由于每次插入的平均执行时间为O(1)，所以建堆平均时间为O(N)
 */
MinPriorityQueue
buildHeap_insert(int *arr, int n)
{
    MinPriorityQueue pqueue;
    
    if (arr == NULL)
    {
        printf("Array is NULL, fail to build heap");
        return NULL;
    }
    
    pqueue = initialize(n * 2);
    for (int i = 0; i < n; i++)
        insert(arr[i], pqueue);
    
    return pqueue;
}

void
printMinPriorityQueue(MinPriorityQueue pqueue)
{
    if (pqueue == NULL)
    {
        PQueueNULLWarning();
        return;
    }
    
    if (pqueue->elements == NULL)
    {
        printf("Fail to print: Elements of priority queue is NULL");
        return;
    }
    
    if (isEmpty(pqueue))
    {
        printf("Empty Prioirty Queue\n");
        return;
    }
    
    printf("Priority Queue\n");
    for (int i = 1; i <= pqueue->size; i++)
        printf("Element[%d] = %d\n", i, pqueue->elements[i]);
    printf("\n");
}
