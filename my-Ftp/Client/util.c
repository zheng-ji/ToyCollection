#include "client.h"

void observeConnect(char *ip) {
	if (Is_Connected == 1) {
		printf("Has established");
		return;
	}
    /*初始化data_socket cmd_socket地址*/
    struct sockaddr_in server_data_addr, server_cmd_addr;
    char buff[BUFFLEN];
	int err;

    data_socket = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_data_addr,0,sizeof(server_data_addr));
    server_data_addr.sin_family = AF_INET;
    server_data_addr.sin_addr.s_addr = inet_addr(ip);
    server_data_addr.sin_port = htons(DATA_PORT);
    err = connect(data_socket,(struct sockaddr*)&server_data_addr,sizeof(server_data_addr));

    cmd_socket = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_cmd_addr,0,sizeof(server_cmd_addr));
    server_cmd_addr.sin_family = AF_INET;
    server_cmd_addr.sin_addr.s_addr = inet_addr(ip);
    server_cmd_addr.sin_port = htons(CMD_PORT);
    err = connect(cmd_socket,(struct sockaddr*)&server_cmd_addr,sizeof(server_cmd_addr));

	if (cmd_socket == -1 || data_socket == -1) {
		printf("socket failed\n");
	}
	Is_Connected = 1;
}

int send_cmd_req(char *cmd) {
	printf("in send_cmd_req %s", cmd);
	if (cmd == NULL) return;
	if (-1 == write(cmd_socket,cmd,strlen(cmd))) {
		printf("write err");
		return SERVER_ACK_ERR;
	}
	char msg[3];
	int rec_len = recv(cmd_socket, msg, 2, MSG_WAITALL);
	msg[2] = 0;
	if (2 == rec_len) {
		if (strcmp(msg, "OK") == 0) {
			return SERVER_ACK_OK;
		}
		if (strcmp(msg, "ER") == 0) {
			return SERVER_ACK_ERR;
		}
	}
	return SERVER_ACK_ERR;
}

void check_connect() {
	if (0 == Is_Connected) {
		printf("still not connected yet!");
	}
}

void say_bye(char *cmd){
	check_connect();
	printf("In say_bye %s", cmd);
	if (send_cmd_req(cmd) == SERVER_ACK_OK) {
		close(data_socket);
		close(cmd_socket);
		printf("disconnect\n");
		Is_Connected = 0;
	}
}

void recv_dir_info(char *cmd) {
	check_connect();
	char buf[2];
	if (send_cmd_req(cmd) == SERVER_ACK_OK) {
		while (-1 != recv(data_socket, buf, 1, MSG_WAITALL)) {
			if (buf[0] == '\0')
				break;
			else {
				buf[1] = '\0';
				printf("%s", buf);
			}
		}
	} else {
		printf("Server return error!\n");
	}
}

void warn () {
	printf("Error command");
}

void req_file(char *input) {
	check_connect();
	printf("In req_file %s", input);
	if (send_cmd_req(input) == SERVER_ACK_OK) {
		char size_str[16];
		char temp[1];
		int i = 0;
		while (-1 != read(data_socket, temp, 1)) {
			if (temp[0] == ' ') {
				size_str[i] = '\0';
				break;
			} else {
				size_str[i] = temp[0];
			}
			i++;
		}
		int size = atoi(size_str);
		char file[size];
		if (-1 == read(data_socket, file, size)) {
			printf("accept file stream error\n");
			return;
		}
		//文件名
		char* filename = input + 4;
		char *p = filename;
		while(1) {
			if (*p == '\n') {
				*p = '\0';
				break;
			}
			p++;
		}
		printf("touch filename:%s size:%d\n", filename, size);
		int fp = open(filename, O_WRONLY | O_TRUNC | O_CREAT);
		if (-1 == write(fp, file, size)) {
			printf("write local file error !\n");
			return;
		}
		else{
			chmod(filename, 0600); //设为可读
			printf("write file ok !\n");
			return ;
		}
	} else {
		printf("file not exist!\n");
		return;
	}
}
