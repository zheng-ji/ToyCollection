#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"

#define MY_KEY 123
#define SEM_KEY 456

int main() {

    int running = 1;
    void *shm = NULL;
    struct shared_msg *shared = NULL;

    //用于保存输入的文本
    char buffer[BUFSIZ + 1];

    // 执行信号量
    int sem_id = 0;

    //创建信号量
    sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);

    if (!set_semvalue(sem_id)) {
        fprintf(stderr, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    //共享内存标识符 int shmid;
    int shmid;

    //创建共享内存
    shmid = shmget(MY_KEY, sizeof(struct shared_msg), 0666|IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, (void*)0, 0);
    if (shm == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", (int)shm);

    //设置共享内存
    shared = (struct shared_msg*)shm;

    //向共享内存中写数据
    while (running) {

        if(!semaphore_p(sem_id))
            exit(EXIT_FAILURE);

        //向共享内存中写入数据
        printf("Enter text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->msg, buffer, TEXT_SZ);

        //写完数据，设置written使共享内存段可读
        shared->isWritten = 1;

        //输入了end，退出循环（程序）
        if(strncmp(buffer, "end", 3) == 0)
            running = 0;

        if(!semaphore_v(sem_id))
            exit(EXIT_FAILURE);
    }

    //把共享内存从当前进程中分离
    if (shmdt(shm) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    exit(EXIT_SUCCESS);
}
