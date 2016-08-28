
## 第五章
1./proc/sys/kernel/pid max 缺省情况下，内核进程ID最大值是32768
2.获取父进程ID getppid(),获取进程ID getpid()
3.如果子进程在父进程之前结束，叫做僵尸进程。只要父进程获取子进程的信息，就会消失，否则会一直保持僵死，Linux内核提供接口了解终止子进程的信息，其中最简单是

```
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait (int *status);
```
