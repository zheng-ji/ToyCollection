#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<pthread.h>
#include<string.h>
#include<time.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5

static void *handle_request(void *argv) {
    int s_c = *((int *)argv);
    time_t now;
    char buff[BUFFLEN];
    int n = 0;
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

static void handle_connect(int s_s) {
    int s_c;
    struct sockaddr_in from;
    int len = sizeof(from);
    pthread_t thread_do;
    while(1) {
        s_c = accept(s_s,(struct sockaddr *)&from,&len);
        if (s_c > 0) {
            int err = pthread_create(&thread_do,NULL,handle_request,(void *)&s_c);
        }
    }
}

int main (int argc,char *argv[]) {
    int s_s;
    struct sockaddr_in local;
    s_s = socket(AF_INET,SOCK_STREAM,0);
    memset(&local,0,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(SERVER_PORT);

    int err = bind(s_s,(struct sockaddr *)&local,sizeof(local));
    err = listen(s_s,BACKLOG);
    handle_connect(s_s);
    close(s_s);
    return 0;
}


