#include<sys/types.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<pthread.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<stdio.h>

#define SERVER_PORT 8888
#define BACKLOG 5
#define CLIENTNUM 1024
#define BUFFLEN 1024

/*可以链接客户端文件描述符的数组*/
int connect_host[CLIENTNUM];
int connect_num = 0;

static void *handle_request(void *argv) {
    time_t now;
    char buff[BUFFLEN];
    int n = 0;

    /*最大文件描述符*/
    int maxfd = -1;
    fd_set scanfd;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    int i = 0;
    int err = 0;
    for (;;) {
        maxfd = -1;
        FD_ZERO(&scanfd);
        for (i = 0; i < CLIENTNUM;i++) {
            if (connect_host[i] != -1) {
                FD_SET(connect_host[i],&scanfd);
                if (maxfd < connect_host[i]) {
                    maxfd = connect_host[i];
                }
            }
        }
        /*select等待*/
        err = select(maxfd + 1,&scanfd,NULL,NULL,&timeout); 
        switch(err) {
            case 0:
                break;
            case -1:
                break;
            default:
                if (connect_num <= 0) {
                    break;
                }
                for (i = 0; i < CLIENTNUM;i++) {
                    /*查找激活文件描述符*/
                    if (connect_host[i] != -1) {
                        if (FD_ISSET(connect_host[i],&scanfd)) {
                            /*清零*/
                            memset(buff,0,BUFFLEN);
                            n = recv(connect_host[i],buff,BUFFLEN,0);
                            if ( n > 0 && !strncmp(buff,"TIME",4)) {
                                memset(buff,0,BUFFLEN);
                                now = time(NULL);
                                sprintf(buff,"%24s\r\n",ctime(&now));
                                send(connect_host[i],buff,strlen(buff),0);
                            }
                            /*更新文件描述符*/
                            connect_host[i] = -1;
                            connect_num --;
                            /*关闭客户端*/
                            close(connect_host[i]);
                        }
                    }
                }
                break;
        }
    }
    return NULL;
}

static void * handle_connect(void * argv) {
    int s_s = *((int*)argv);
    int s_c = -1;
    struct sockaddr_in from;
    int len = sizeof(from);

    //接受客户端连接
    for (;;) {
        int i = 0;
        int s_c = accept(s_s,(struct sockaddr *)&from,&len);
        //接受客户端请求
        printf("a client connect from:%s\n",inet_ntoa(from.sin_addr));
        /*找到合适,将文件描述符放入*/
        for (i = 0; i < CLIENTNUM;i++) {
            if (connect_host[i] == -1) {
                /*放入*/
                connect_host[i] = s_c;
                /*客户端计数器+1*/
                connect_num ++;
                /*继续轮询，等待客户端连接*/
                break;
            }
        }
    }
    return NULL;
}

int main(int argc,char *argv[]) {

    int s_s;
    struct sockaddr_in local;
    int i = 0;
    memset(connect_host,-1,CLIENTNUM);

    /*建立tcp套接字*/
    s_s = socket(AF_INET,SOCK_STREAM,0);

    /*初始化地址*/
    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(SERVER_PORT);
    int err = bind(s_s,(struct sockaddr*)&local,sizeof(local));
    err = listen(s_s,BACKLOG);

    pthread_t thread_do[2];
    pthread_create(&thread_do[0],NULL,handle_connect,(void *)&s_s);
    pthread_create(&thread_do[1],NULL,handle_request,NULL);
    for (i = 0; i < 2; i++) {
        pthread_join(thread_do[i],NULL);
    }
    close(s_s);
    return 0;
}

