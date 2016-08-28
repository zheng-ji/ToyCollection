## 第五章

- 缺省情况下，内核进程ID最大值是32768, 

```
cat /proc/sys/kernel/pid max 
```

- 获取父进程ID getppid(),获取进程ID getpid()
- 如果子进程在父进程之前结束，叫做僵尸进程。只要父进程获取子进程的信息，就会消失，否则会一直保持僵死，Linux 内核提供接口了解终止子进程的信息。

```
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait (int *status);
```

- 守护进程
守护进程必须是 init 进程的子进程，不予任何控制终端关联

+ 调用 fork()
+ 在守护进程的父进程调用 exit(),使得祖父进程确认父进程已经结束。父进程不再继续运行
+ 调用 setsid(), 使得守护进程有一个新的进程组和新会话，两者把它作为首进程，保证它不会与控制终端相关联。
+ 用 chidir() 将氮气工作目录改为跟某，
+ 关闭所有文件描述符，不需要继承任何打开的文件描述符。
+ 打开0,1,2号文件描述符,重定向到 /dev/null

```
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fs.h>
int main (void)
{
    pid_t pid;
    int i;
    /* create new process */
    pid = fork ( );
    if (pid == -1)
        return -1;
    else if (pid != 0)
        exit (EXIT_SUCCESS);
    /* create new session and process group */
    if (setsid ( ) == -1)
        return -1;
    /* set the working directory to the root directory */
        if (chdir (”/”) == -1)
            return -1;
    /* close all open files--NR_OPEN is overkill, but works */
    for (i = 0; i < NR_OPEN; i++)
        close (i);
    /* redirect fd’s 0,1,2 to /dev/null */
    ￼￼  open (”/dev/null”, O_RDWR);
    dup (0);
    dup (0);
    /* do its daemon thing... */
    return 0;
}
```
