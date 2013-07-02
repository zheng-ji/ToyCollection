#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<stdio.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888

int main(int argc,char *argv[]) {

    int s;
    struct sockaddr_in server;
    char buff[BUFFLEN];
    int n = 0;

    /*建立TCP套接字*/
    s = socket(AF_INET,SOCK_STREAM,0);

    /*初始化地址*/
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(SERVER_PORT);

    /*连接服务器*/
    int err = connect(s,(struct sockaddr*)&server,sizeof(server));
    memset(buff,0,BUFFLEN);
    strcpy(buff,"TIME");
    /*发送数据*/
    send(s,buff,strlen(buff),BUFFLEN);
    memset(buff,0,BUFFLEN);

    /*接收数据*/
    n = recv(s,buff,BUFFLEN,0);

    /*打印消息*/
    if (n > 0) 
        printf("TIME:%s",buff);
    close(s);
    return 0;
}
