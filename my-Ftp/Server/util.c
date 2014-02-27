#include "server.h"

void ack_succ(int net_cmd_sock) {
	char ack[CMD_LEN];
	sprintf(ack, "%s", "OK");
	send(net_cmd_sock, ack, strlen(ack), 0);
}

void ack_err(int net_cmd_sock) {
	char ack[CMD_LEN];
	sprintf(ack, "%s", "ER");
	send(net_cmd_sock, ack, strlen(ack), 0);
}

void close_svr(int net_cmd_sock, int net_data_sock) {
	ack_succ(net_cmd_sock);
	sleep(2);
	close(net_cmd_sock);
	close(net_data_sock);
	printf("close server\n");
}

int get_file_size(char * strFileName)   
{  
    FILE * fp = fopen(strFileName, "r");  
    fseek(fp, 0L, SEEK_END);  
    int size = ftell(fp);  
    fclose(fp);  
	return size;
}

void send_dir_info(int net_cmd_sock, int net_data_sock) {
	printf("svr send dir info");
	ack_succ(net_cmd_sock);
	system("ls>dirent.list");
	int BLOCK_LEN = 128;
	char buf[BLOCK_LEN];
	int fp = open("dirent.list", O_RDONLY);
	int length;
	while (0 != (length = read(fp, buf, BLOCK_LEN))) {
		buf[length] = '\0';
		printf("%s", buf);
		write(net_data_sock, buf, BLOCK_LEN);
	}
	system("rm dirent.list");
}

void send_file(char* filename, int net_data_sock, int net_cmd_sock) {
	int fp = open(filename, O_RDONLY);
	if (fp == -1) {
		printf("error file\n");
		ack_err(net_cmd_sock);
	} else {
		ack_succ(net_cmd_sock);
		int filesize = get_file_size(filename);
		char size_str[16];
		sprintf(size_str, "%d", filesize);

		long total_buf_len = strlen(size_str) + 1 + filesize;//需要发送数据位数，空格，数据
		char buf[total_buf_len];
		strcpy(buf,size_str);
		buf[strlen(size_str)]=' ';

		int length = read(fp,buf + strlen(size_str) + 1,filesize);
		close(fp);
		if(length < 0){
			printf("read file error!\n");
			return ;
		} else {
			write(net_data_sock,buf,total_buf_len);
			return ;
		}
	}
}
