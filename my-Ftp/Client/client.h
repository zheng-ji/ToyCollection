#ifndef __CLIENT_H__
#define __CLIENT_H__

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#define BUFFLEN 1024
#define DATA_PORT 8888
#define CMD_PORT 8889
#define SERVER_ACK_ERR 0
#define SERVER_ACK_OK 1
#define BACKLOG 10

int Is_Connected;
int data_socket;
int cmd_socket;

int IntiCmdSession(char *input);
void observeConnect(char *ip);
int send_cmd_req(char *cmd);
void check_connect();
void say_bye(char *cmd);
void recv_dir_info(char *cmd);
void warn ();
void req_file(char *input);
void input_server_ip();


#endif
