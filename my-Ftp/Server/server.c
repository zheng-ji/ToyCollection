#include"server.h"

static void handle_request(int net_data_sock, int net_cmd_sock) {
	while(1) {
		char cmd[CMD_LEN];
		int n = 0;
		n = recv(net_cmd_sock, cmd, 3, MSG_WAITALL);
		cmd[3] = '\0';
		printf("rec %d byte, msg: %s\n", n, cmd);
		if (n > 0) {
			printf(cmd);
			if (0 == strcmp(cmd, "BYE")) {
				close_svr(net_cmd_sock);
			}
			if (0 == strcmp(cmd, "DIR")) {
				send_dir_info(net_cmd_sock, net_data_sock);
			}
			if (0 == strcmp(cmd, "GET")) {
				char buf[128];
				n = read(net_cmd_sock, buf, 128); //不用recv ,继续read
				buf[n] = '\0';
				char *filename = buf + 1; //去除1个空格
				char *p = filename;
				while(1) {
					if (*p == '\n') {
						*p = '\0';
						break;
					}
					p++;
				}
				send_file(filename, net_data_sock, net_cmd_sock);
			}
		} else {
			break;
		}
	}
}

static void handle_connect(int svr_data_sock, int svr_cmd_sock) {
	int net_data_sock;
	int net_cmd_sock;
	struct sockaddr_in from_cmd;
	struct sockaddr_in from_data;
	int len = sizeof(struct sockaddr_in);
	while(1) {
		net_data_sock = accept(svr_data_sock,(struct sockaddr *)&from_data,&len);
		net_cmd_sock = accept(svr_cmd_sock,(struct sockaddr *)&from_cmd,&len);
		if (-1 == net_data_sock || -1 == net_cmd_sock) {
			printf("accept err !\n");
			return;
		}
		printf("connect from %#x : %#x\r\n",
				ntohl(from_data.sin_addr.s_addr), ntohs(from_data.sin_port));
		printf("connect from %#x : %#x\r\n",
				ntohl(from_cmd.sin_addr.s_addr), ntohs(from_cmd.sin_port));
		pid_t p_id = fork();
		if (p_id == 0) {
			handle_request(net_data_sock, net_cmd_sock);
		} else if (p_id < 0) {
			/* close(svr_data_sock); */
			return ;
		}
	}
	close(svr_data_sock);
}

int main(int argc,char * argv[]) {
	int svr_data_sock;
	int svr_cmd_sock;
	int err;

	//svr_data_sock init
	struct sockaddr_in data_sock_addr;
	svr_data_sock = socket(AF_INET,SOCK_STREAM,0);
	memset(&data_sock_addr,0,sizeof(data_sock_addr));
	data_sock_addr.sin_family = AF_INET;
	data_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	data_sock_addr.sin_port = htons(DATA_PORT);
	err = bind(svr_data_sock,(struct sockaddr *)&data_sock_addr,sizeof(data_sock_addr));
	err = listen(svr_data_sock,BACKLOG);

	//svr_cmd_sock init
	struct sockaddr_in cmd_sock_addr;
	svr_cmd_sock = socket(AF_INET,SOCK_STREAM,0);
	memset(&cmd_sock_addr,0,sizeof(cmd_sock_addr));
	cmd_sock_addr.sin_family = AF_INET;
	cmd_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	cmd_sock_addr.sin_port = htons(CMD_PORT);
	err = bind(svr_cmd_sock,(struct sockaddr *)&cmd_sock_addr,sizeof(cmd_sock_addr));
	err = listen(svr_cmd_sock,BACKLOG);

	handle_connect(svr_data_sock, svr_cmd_sock);
	return 0;
}


