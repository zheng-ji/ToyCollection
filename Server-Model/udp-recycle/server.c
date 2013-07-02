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
    struct sockaddr_in local,to;
    time_t now;
    char buff[BUFFLEN];
    int n = 0;
    int len = sizeof(to);

    s = socket(AF_INET,SOCK_DGRAM,0);

    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(SERVER_PORT);

    int err = bind(s,(struct sockaddr*)&local,sizeof(local));

    while(1) {
        memset(buff,0,BUFFLEN);
        n = recvfrom(s,buff,BUFFLEN,0,(struct sockaddr *)&to,&len);
        if (n > 0 && !strncmp(buff,"TIME",4)) {
            memset(buff,0,BUFFLEN);
            now = time(NULL);
            sprintf(buff,"%24s\r\n",ctime(&now));
            sendto(s,buff,strlen(buff),0,(struct sockaddr *)&to,len);
        }
    }
    close(s);
    return 0;
}

