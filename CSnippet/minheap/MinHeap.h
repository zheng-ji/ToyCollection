#ifndef DataStructures_MinHeap_h
#define DataStructures_MinHeap_h

struct MinHeap;
typedef struct MinHeap * MinPriorityQueue;
typedef int ElementType;

// 初始化堆
MinPriorityQueue initialize(int maxElements);

// 销毁堆
void destroy(MinPriorityQueue pqueue);

// 清空堆中的元素
void makeEmpty(MinPriorityQueue pqueue);

// 插入操作
void insert(ElementType x, MinPriorityQueue pqueue);

// 删除最小者操作，返回被删除的堆顶元素
ElementType deleteMin(MinPriorityQueue pqueue);

// 查找最小者（堆顶）
ElementType findMin(MinPriorityQueue pqueue);

// 判断堆是否为空
int isEmpty(MinPriorityQueue pqueue);

// 判断堆是否满
int isFull(MinPriorityQueue pqueue);

// 通过一个数组来建堆，相当于将用数组实现的无序树转换为堆序树
MinPriorityQueue buildHeap_insert(int *arr, int n);
MinPriorityQueue buildHeap_percolate(int *arr, int n);

// 打印堆
void printMinPriorityQueue(MinPriorityQueue pqueue);

#endif
