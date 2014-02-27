#include "client.h"


int main(int argc,char *argv[]) {
    char input[50];
    printf("Client:\n");
    Is_Connected = 0;
    input_server_ip();
    while (1) {
        printf(">");
        gets(input);
        char command[4];
        strncpy(command, input, 3);
        command[3] = '\0';
        printf("cmd is:%s \n",command);
        if (!strcmp(command, "BYE")) {
            say_bye(command);
        }
        if (!strcmp(command, "DIR")) {
            recv_dir_info(command);
        }
        if (!strcmp(command, "GET")) {
            req_file(input);
        }
        if (!strcmp(command, "PRT")) {
            observeConnect(input);
        }
    }
    close(cmd_socket);
    close(data_socket);
    return 0;
}

