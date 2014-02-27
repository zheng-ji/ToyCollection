#include "client.h"

void input_server_ip() {
    char input[50];
    do {
        printf("Enter server ip:");
        gets(input);
        printf("connect to ip: %s\n", input);
    } while (InitCmdSession(input));
}
int InitCmdSession(char *input) {
    struct sockaddr_in server_cmd_addr;
    cmd_socket = socket(AF_INET,SOCK_STREAM,0);
    memset(&server_cmd_addr,0,sizeof(server_cmd_addr));
    server_cmd_addr.sin_family = AF_INET;
    server_cmd_addr.sin_addr.s_addr = inet_addr(input);
    server_cmd_addr.sin_port = htons(CMD_PORT);
    if (-1 == connect(cmd_socket,(struct sockaddr*)&server_cmd_addr,sizeof(server_cmd_addr)) ||
        -1 == cmd_socket) {
		printf("socket failed\n");
        return -1;
    } else {
        return 0;
    }
}

void observeConnect(char *input) {
	if (Is_Connected == 1) {
		printf("Has established\n");
		return;
	}

    int svr_data_sock;
    struct sockaddr_in data_sock_addr;
    svr_data_sock = socket(AF_INET,SOCK_STREAM,0);
    memset(&data_sock_addr,0,sizeof(data_sock_addr));
    data_sock_addr.sin_family = AF_INET;
    data_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    data_sock_addr.sin_port = htons(atoi(input + 4));
    if (-1 == bind(svr_data_sock,(struct sockaddr *)&data_sock_addr,sizeof(data_sock_addr))) {
        printf("observeConnect err in bind\n ");
    }

    if (-1 == listen(svr_data_sock,BACKLOG)) {
        printf("observeConnect err in listen\n ");
    }

    if (send_cmd_req(input) == SERVER_ACK_OK) {
        struct sockaddr_in from_data;
        socklen_t len = sizeof(struct sockaddr_in);
        data_socket = accept(svr_data_sock,(struct sockaddr *)&from_data,&len);
        if ( -1 == data_socket) {
            printf("accept err !\n");
            return;
        }
    }
    Is_Connected = 1;
}

int send_cmd_req(char *cmd) {
	printf("in send_cmd_req %s \n", cmd);
	if (cmd == NULL) 
        return SERVER_ACK_ERR;
	if (-1 == write(cmd_socket,cmd,strlen(cmd))) {
		printf("write err.\n");
		return SERVER_ACK_ERR;
	}
	char msg[3];
	int rec_len = recv(cmd_socket, msg, 2, MSG_WAITALL);
	msg[2] = '\0';
	if (2 == rec_len) {
		if (strcmp(msg, "OK") == 0) {
            printf("ok\n");
			return SERVER_ACK_OK;
		}
		if (strcmp(msg, "ER") == 0) {
            printf("err\n");
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
    char tmp[128];
    read(data_socket, tmp, 128);
}

void warn () {
	printf("Error command.\n");
}

void req_file(char *input) {
	check_connect();
	printf("In req_file %s\n", input);
	if (send_cmd_req(input) == SERVER_ACK_OK) {
		char size_str[16];
		char temp[1];
		int i = 0;
		while (-1 != read(data_socket, temp, 1)) {
			if (temp[0] == ' ') {
				size_str[i] = '\0';
				break;
			} else {
                //flush data_socket
                if (temp[0] - '0' > 10 || temp[0] - '0' < 0 )
                    continue;
                else
                    size_str[i] = temp[0];
			}
			i++;
		}
        printf("file size:%s", size_str);
		int size = atoi(size_str);
		char file[size];
		if (-1 == read(data_socket, file, size)) {
			printf("accept file stream error\n");
			return;
		}
		//文件名
		char* filename = input + 4;
		char *p = filename;
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
