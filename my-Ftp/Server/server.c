#include"server.h"

int connect_data_channel(int data_port) {
    struct sockaddr_in server_data_addr;
    int net_data_socket = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_data_addr,0,sizeof(server_data_addr));
    server_data_addr.sin_family = AF_INET;
    server_data_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_data_addr.sin_port = htons(data_port);
    if (-1 != connect(net_data_socket,(struct sockaddr*)&server_data_addr,sizeof(server_data_addr))) {
        return net_data_socket;
    } 
}

static void handle_request(int net_cmd_sock) {
    while(1) {
        int net_data_sock;
        char cmd[CMD_LEN];
        char buf[128];
        char *p;
        int n = 0;
        n = recv(net_cmd_sock, cmd, 3, MSG_WAITALL);
        cmd[3] = '\0';
        printf("rec %d byte, msg: %s\n", n, cmd);
        if (n > 0) {
            if (0 == strcmp(cmd, "PRT")) {
                n = read(net_cmd_sock, buf, 128); //不用recv ,继续read
                buf[n] = '\0';
                char *port = buf + 1; //去除1个空格
                ack_succ(net_cmd_sock);
                net_data_sock = connect_data_channel(atoi(port));
            }
            if (0 == strcmp(cmd, "BYE")) {
                close_svr(net_cmd_sock, net_data_sock);
            }
            if (0 == strcmp(cmd, "DIR")) {
                send_dir_info(net_cmd_sock, net_data_sock);
            }
            if (0 == strcmp(cmd, "GET")) {
                n = read(net_cmd_sock, buf, 128); //不用recv ,继续read
                buf[n] = '\0';
                char *filename = buf + 1; //去除1个空格
                p = filename;
                send_file(filename, net_data_sock, net_cmd_sock);
            }
        } else {
            break;
        }
    }
}

static void handle_connect(int svr_cmd_sock) {
    int net_data_sock;
    int net_cmd_sock;
    struct sockaddr_in from_cmd;
    socklen_t len = sizeof(struct sockaddr_in);
    while(1) {
        net_cmd_sock = accept(svr_cmd_sock,(struct sockaddr *)&from_cmd,&len);
        if ( -1 == net_cmd_sock) {
            printf("accept err !\n");
            return;
        }
        printf("connect from %s : %d\r\n", inet_ntoa(from_cmd.sin_addr), ntohs(from_cmd.sin_port));
        pid_t p_id = fork();
        if (p_id == 0) {
            handle_request(net_cmd_sock);
        } else if (p_id < 0) {
            return ;
        }
    }
}

int main(int argc,char * argv[]) {
    int svr_cmd_sock;

    //svr_cmd_sock init
    struct sockaddr_in cmd_sock_addr;
    svr_cmd_sock = socket(AF_INET,SOCK_STREAM,0);
    memset(&cmd_sock_addr,0,sizeof(cmd_sock_addr));
    cmd_sock_addr.sin_family = AF_INET;
    cmd_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    cmd_sock_addr.sin_port = htons(CMD_PORT);
    if (-1 == bind(svr_cmd_sock,(struct sockaddr *)&cmd_sock_addr,sizeof(cmd_sock_addr))) {
        printf("svr cmd socket bind err\n");
    }
    if (-1 == listen(svr_cmd_sock,BACKLOG)) {
        printf("svr cmd socket listen err\n");
    }

    /*
       if (fork() != 0)
       exit(1);
       if (setsid() < 0)
       exit(1);
       if (fork() != 0)
       exit(1);
       if (chdir("/tmp") == -1)
       exit(1);
       for (int fd = 0, fdtablesize = getdtablesize(); fd < fdtablesize; fd++)
       close(fd);

       umask(0);
       */

    handle_connect(svr_cmd_sock);
    return 0;
}
