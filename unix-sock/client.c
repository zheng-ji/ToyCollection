#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<errno.h>

//define send and recv buf size
#define BUFSIZE 250*1024

//define unix domain socket path
#define pmapi "pmapi"

int main(int argc, char** argv)
{
    char tx_buf[BUFSIZE];
    int pmapi_fd, ret;
    socklen_t len;
    struct sockaddr_un pmmanager_addr, pmapi_addr;

    //create pmmanager socket fd
    pmapi_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(pmapi_fd == -1)
    {
        perror("cannot create pmapi fd.");
    }

    unlink(pmapi);
    //configure pmapi's addr
    memset(&pmapi_addr, 0, sizeof(pmapi_addr));
    pmapi_addr.sun_family = AF_UNIX;
    strncpy(pmapi_addr.sun_path, pmapi, sizeof(pmapi_addr.sun_path)-1);
    //bind pmapi_fd to pmapi_addr
    ret = bind(pmapi_fd, (struct sockaddr*)&pmapi_addr, sizeof(pmapi_addr));
    if(ret == -1)
    {
        perror("bind error.");
    }

    int sendBufSize;
    len = sizeof(sendBufSize);
    ret = getsockopt(pmapi_fd, SOL_SOCKET, SO_SNDBUF, &sendBufSize, &len);
    if(ret ==-1)
    {
        perror("getsocket error.");
    }
    printf("Before setsockopt, SO_SNDBUF-%d\n",sendBufSize); 
    sendBufSize = 512*1024;
    ret = setsockopt(pmapi_fd, SOL_SOCKET, SO_SNDBUF, &sendBufSize, len);
    if(ret == -1)
    {
        perror("setsockopt error.");
    }
    ret = getsockopt(pmapi_fd, SOL_SOCKET, SO_SNDBUF, &sendBufSize, &len);
    if(ret ==-1)
    {
        perror("getsocket error.");
    }
    printf("Set send buf successful, SO_SNDBUF-%d\n\n\n", sendBufSize); 

    //configure pmmanager's addr
    memset(&pmmanager_addr, 0, sizeof(pmmanager_addr));
    pmmanager_addr.sun_family = AF_UNIX;
    strncpy(pmmanager_addr.sun_path, pmmanager, sizeof(pmmanager_addr)-1);
    len = sizeof(pmmanager_addr);

    int sendSize = 0;
    int i;
    for(i=1; i<=4; i++)
    {
        memset(tx_buf, '0', sizeof(tx_buf));
        sprintf(tx_buf, "send msg %d to pmmanager.", i);
        printf("%s, msg size - %d\n",tx_buf, sizeof(tx_buf));
        sendSize = sendto(pmapi_fd, tx_buf, sizeof(tx_buf), 0, (struct sockaddr*)&pmmanager_addr, len);
        if(sendSize == -1)
        {
            perror("sendto error.");
        }
        printf("Send message to pmmanager: %s\n\n\n", tx_buf);
    }
    return 0;
}
