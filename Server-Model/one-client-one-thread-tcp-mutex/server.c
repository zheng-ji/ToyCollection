#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<stdio.h>
#include<pthread.h>
#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5
#define CLIENTNUM 2

pthread_mutex_t ALOCK = PTHREAD_MUTEX_INITIALIZER;
static void * handle_request(void *argv) {
    int s_s = *((int *)argv);
    int s_c;
    struct sockaddr_in from;
    int len = sizeof(from);
    for (;;) {
        time_t now;
        char buff[BUFFLEN];
        int n = 0;
        pthread_mutex_lock(&ALOCK);
        s_c = accept(s_s,(struct sockaddr *)&from,&len);
        pthread_mutex_unlock(&ALOCK);
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
    return NULL;
}

static void handle_connect(int s) {
    int s_s = s;
    pthread_t thread_do[CLIENTNUM];
    int i = 0;
    for (i = 0; i < CLIENTNUM;i++) {
        pthread_create(&thread_do[i],NULL,handle_request,(void*)&s_s);
    }
    for (i = 0; i < CLIENTNUM;i++) {
        pthread_join(thread_do[i],NULL);
    }
}

int main() {
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
