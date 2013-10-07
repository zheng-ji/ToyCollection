#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<stdio.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5

int main(int argc,char *argv[]) {
    int s_s,s_c;
    struct sockaddr_in local,from;
    time_t now;
    char buff[BUFFLEN];
    int n = 0;
    int len = sizeof(from);

    /*建立TCP套接字*/
    s_s = socket(AF_INET,SOCK_STREAM,0);

    /*初始化地址*/
    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(SERVER_PORT);

    /*连接服务器*/
    int err = bind(s_s,(struct sockaddr *)&local,sizeof(local));
    err = listen(s_s,BACKLOG);

    /*主处理过程*/
    while(1) {
        /*接受客户端连接*/
        s_c = accept(s_s,(struct sockaddr *)&from,&len);
        memset(buff,0,BUFFLEN);
        n = recv(s_c,buff,BUFFLEN,0);
        if (n > 0 && !strncmp(buff,"TIME",4)) {
            memset(buff,0,BUFFLEN);
            now = time(NULL);
            sprintf(buff,"%24s\r\n",ctime(&now));
            send(s_c,buff,strlen(buff),0);
        }
        close(s_c);
    }

    close(s_s);
    return 0;
}
