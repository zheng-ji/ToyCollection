#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<errno.h>

//define send and recv buf size
#define BUFSIZE 512*1024

//define unix domain socket path
#define pmmanager   "/tmp/pmmanager"
#define pmapi       "/tmp/pmapi"

int main(int argc, char** argv)
{
    char rx_buf[BUFSIZE];
    int pmmanager_fd, ret;
    socklen_t len;
    struct sockaddr_un pmmanager_addr, pmapi_addr;

    //create pmmanager socket fd
    pmmanager_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(pmmanager_fd == -1)
    {
        perror("cannot create pmmanager fd.");
    }

    unlink(pmmanager);
    memset(&pmmanager_addr, 0, sizeof(pmmanager_addr));
    pmmanager_addr.sun_family = AF_UNIX;
    strncpy(pmmanager_addr.sun_path, pmmanager, sizeof(pmmanager_addr.sun_path)-1);

    //bind pmmanager_fd to pmmanager_addr
    ret = bind(pmmanager_fd, (struct sockaddr*)&pmmanager_addr, sizeof(pmmanager_addr));
    if(ret == -1)
    {
        perror("can not bind pmmanager_addr");
    }

    int recvBufSize;
    len = sizeof(recvBufSize);
    ret = getsockopt(pmmanager_fd, SOL_SOCKET, SO_RCVBUF, &recvBufSize, &len);
    if(ret ==-1)
    {
        perror("getsocket error.");
    }
    printf("Before setsockopt, SO_RCVBUF-%d\n",recvBufSize); 
    recvBufSize = 512*1024;
    ret = setsockopt(pmmanager_fd, SOL_SOCKET, SO_RCVBUF, &recvBufSize, len);
    if(ret == -1)
    {
        perror("setsockopt error.");
    }
    ret = getsockopt(pmmanager_fd, SOL_SOCKET, SO_RCVBUF, &recvBufSize, &len);
    if(ret ==-1)
    {
        perror("getsocket error.");
    }
    printf("Set recv buf successful, SO_RCVBUF-%d\n",recvBufSize); 

    int recvSize;
    memset(&pmapi_addr, 0, sizeof(pmapi_addr));
    len = sizeof(pmapi_addr);
    printf("==============wait for msg from pmapi====================\n");
    for(;;)
    {
        memset(rx_buf, 0, sizeof(rx_buf));
        recvSize = recvfrom(pmmanager_fd, rx_buf, sizeof(rx_buf), 0, (struct sockaddr*)&pmapi_addr, &len);
        if(recvSize == -1)
        {
            perror("recvfrom error.");
        }
        printf("Recved message from pmapi: %s\n", rx_buf);
    }
    return 0;
}
