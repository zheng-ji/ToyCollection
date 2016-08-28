## 第八章 内存管理

- Linux 将他的物理内存虚拟化，进程并不能直接在物理内存上寻址，内核为每个进程维护一个特殊的虚拟地址空间，这个空间是线性的.

- 页和页面调度 
 + 典型页大小包括4k(32bit系统)，8k (64bit 系统)
 + 虚存中的多个页面，甚至是不同进程的虚拟地址空间，可能被映射到同一个物理页，允许不同的虚拟地址空间共享物理内存上的数据，当一个进程试图写某个共享的可写页，内核会透明的创造一份这个页的拷贝（copy-on-write）

- 存储器区域
 + 文本段包含一个进程的代码，字符串，常量和只读数据
 + 堆栈段包含一个进程的执行栈，局部变量，函数返回值
 + 数据段，又叫堆，包含进程的动态存储空间，可写
 + BSS 包含没有初始化的全局变量

- 动态内存分配
 + 数组分配，calloc 将会分配的区域全部用0进行初始化, malloc 需要用memset 初始化
 + realloc， 调整已分配内存大小，返回的可能不是原有指针，申请新内存，再拷贝

```
#include <stdlib.h>
void *calloc (size_t nr, size_t size);
void *realloc (void *ptr, size_t size);
```

- 对齐
 + 一个结构体的对齐要求和成员中最大的那个类型一样

- 栈上分配内存 
 + 不必释放分配的内存
 + 随着栈的收缩自动释放，alloc 返回的内存不能继续使用

```
#include <alloca.h>
void * alloca (size_t size);
```

- 存储器操作

```
#include <string.h>
// 内存清零并返回s
void * memset (void *s, int c, size_t n);
// 比较s1 s2 前 n  个字节
int memcmp (const void *s1, const void *s2, size_t n);
// 复制 src 的 前n个字符
void * memmove (void *dst, const void *src, size_t n);
```

- 超量使用内存
 + vm.overcommit memory 默认值是0，告诉内核执行适度的超量使用策略， 如果为1，将确定所有的分配请求, 如果为2，将关闭所有的过量请求, 承诺的内存大小被限制在交换空间和可调内存的大小，默认 vm.overcommit ratio = 50，物理内存放着内核，页表，系统保留页，锁定页面



- 处理器亲缘性
 + 如果一个进程曾在某一cpu上运行，进程调度器尽量把它放在同一CPU, 处理器的进程迁移会带来性能损失. 最大的损失来自于迁移带来的缓存效应
 + soft affinity 表明调度器持续调度进程到同一个 CPU 的自然倾向
 + hard affinity 强制内核保证进程到处理器的绑定
