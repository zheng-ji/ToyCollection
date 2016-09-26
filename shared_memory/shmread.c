#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"

#define MY_KEY 123 
#define SEM_KEY 456

int main() {   

    //程序是否继续运行的标志
    int running = 1;

    //分配的共享内存的原始首地址
    void *shm = NULL;

    //指向shm
    struct shared_msg *shared;

    // 执行信号量
    int sem_id = 0;

    //创建信号量
    sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        if (errno == EEXIST) {
            sem_id = semget(SEM_KEY, 1, 0666);
            if (sem_id == -1) {
                fprintf(stderr, "semget failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (!set_semvalue(sem_id)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    //共享内存标识符 int shmid;
    int shmid;

    //创建共享内存, IPC_CREATE 创建
    shmid = shmget(MY_KEY, sizeof(struct shared_msg), 0666|IPC_CREAT);
    if (shmid == -1) {
        if (errno == EEXIST) {
            shmid = shmget(MY_KEY, sizeof(struct shared_msg), 0666);
            if (shmid == -1)  {
                fprintf(stderr, "shmget failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    //将共享内存连接到当前进程的地址空间, 第二个参数是0表示让操作系统选择地址
    shm = shmat(shmid, 0, 0);
    if (shm == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\nMemory attached at %X\n", (int)shm);

    //设置共享内存
    shared = (struct shared_msg*)shm;
    shared->isWritten= 0;

    //读取共享内存中的数据
    while (running) {

        if(!semaphore_p(sem_id))
            exit(EXIT_FAILURE);

        if (shared->isWritten != 0) {


            printf("You wrote: %s", shared->msg);
            sleep(rand() % 3);
            shared->isWritten = 0;
            if (strncmp(shared->msg, "end", 3) == 0) {
                running = 0;
            }

        }
        if(!semaphore_v(sem_id))
            exit(EXIT_FAILURE);
    }

    //把共享内存从当前进程中分离
    if (shmdt(shm) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    //删除共享内存
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
