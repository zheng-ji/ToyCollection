#ifndef __SERVER_H__
#define __SERVER_H__

#include<sys/types.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<string.h>
#include<time.h>
#include<netinet/in.h>
#include <fcntl.h>

#define BUFFLEN 1024
#define BACKLOG 5
#define DATA_PORT 8888
#define CMD_PORT 8889
#define CMD_LEN 4


void ack_succ(int net_cmd_sock);
void ack_err(int net_cmd_sock);
void close_svr(int net_cmd_sock);
int get_file_size(char * strFileName);  
void send_dir_info(int net_cmd_sock, int net_data_sock);
void send_file(char* filename, int net_data_sock, int net_cmd_sock);

#endif
