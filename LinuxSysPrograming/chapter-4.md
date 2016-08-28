## 第四章 高级文件IO

1. poll，select 每次调用时内核必须遍历所有被监视的文件描述符，每次遍历成为明显的瓶颈。

2. EPOLL API操作

2.1 int epoll_create(int size) 创建 epoll 上下文，size 告诉内核需要监听的文件描述符的数目。
2.2 epoll_ctl() 向指定的epoll上下文中加入或者删除，修改文件描述符。
2.3 epoll_wait() 等待给定epoll实例关联的文件描述符上事件，时限为 timeout 毫秒，成功返回，且最多有多个 maxevents
2.4 data 是用户使用，确定监听事件后，data 会返回给用户，通常将event.data.fd设定为fd， 这样就知道那个文件描述符触发时间。
2.5 OP 是要指定的操作
2.6 EVENT 订阅的事件

```
#include<sys/epoll.h>
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait (int epfd, struct epoll_event *events, int maxevents, int timeout);

struct epoll_event {
	__u32 events;
	union {
		void *ptr;
		int fd;
		__u32 u32;
		__u64 u64;
	} data
}

struct epoll_event event;
int ret;
event.data.fd = fd; event.events = EPOLLIN;
ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event);
if (ret) perror ("epoll_ctl");

// OP
EPOLL_CTL_ADD 把fd指定的文件添加到epfd指定的epoll实例监听集中
EPOLL_CTL_DEL 把fd指定的文件从epfd指定的epoll集中删除
EPOLL_CTL_MOD 使用event 改变现有fd的监听行为

// EVENT
EPOLLERR: 文件出错。默认是有监听的
EPOLLET:在监听文件开启边沿触发
EPOLLHUP 被挂起
EPOLLIN 文件未阻塞，可读
EPOLLOUT 文件未阻塞，可写
```

3. 边沿触发事件和水平触发


* 步骤1 生产者想管道写入1kb数据，
* 步骤2 消费者在官道上调用 epoll_wait,等待 pipe 出现数据，从而可读

对于水平触发的监听，步骤2 里对 epoll_wait() 的调用将立即返回，以表明 pipe 可读。
对于边沿触发的监听，这个调用直到步骤1发生后才返回。也就是说，即使调用 epoll_wait 管道已经可读，调用仍然会等待直到有数据可写入，之后返回。

水平触发是默认行为，是大多数开发者期望的，边缘触发需要不同的方式来写程序。
记忆方式：水平触发放生在状态发生是触发，关心的是状态，边缘触发只有状态改变的时候才产生，关心的是事件本身。

4. 除了标准文件I/O， 允许应用程序将文件映射到内存中，内存和文件数据意义对应，可以通过内存来访问文件

```
#include <sys/mman.h>
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
```

5. mmap 的优缺点

* mmap 的优点：相对于read, write 需要从用户缓冲区进行数据读写，从而使用映射文件进行操作，避免多余的数据拷贝，在映射对象中搜索只需要一般的指针操作，不必使用lseek
* mmap 的缺点：映射取悦通常是页大小的整数倍，较大比重的空间会被浪费，存储映射区域必须在进程地址空间内，对于32位的地址空间，大量大小各异的银蛇会导致大量的碎片出现，很难找到连续的大片空内存。

6. 预读对随机访问来讲是无用的开销

7. I/O调度器实现两个基本操作：合并和排序，合并操作将两个或多个相邻的请求的过程合并为一个，总的吞吐量一样，但次数少了。排序操作，按照块好递增的顺序重新安排等待的IO。

8. Deadline IO 调度器，有读 FIFO 队列，写 FIFO 队列，队列中按请求提交时间排序，读队列过期时间是500毫秒，写对了是5秒，当一个新的IO请求提交后，被按需插入到标准队列，然后加入到响应读写队列， 如果FIFO队列头的请求超出了队列的过期时间，IO调度器停止从标准IO队列调度请求，转而调度这个FIFO队列的对首请求。

```
//查看scheduler
cat /sys/block/xvda/queue/scheduler 
```
