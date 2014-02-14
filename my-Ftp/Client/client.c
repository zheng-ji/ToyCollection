#include "client.h"


int main(int argc,char *argv[]) {
	char input[50];
	printf("Client:\n");
	Is_Connected = 0;
	while (1) {
		printf(">");
		fgets(input, 50, stdin);
		char command[3];
		strncpy(command, input, 3);
		/* command[4] = '\0'; */
		printf("cmd is:%s\n",command);
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
			char *ip = input + 4;
			observeConnect(ip);
		} else {
			warn();
		}
	}
    close(cmd_socket);
    close(data_socket);
    return 0;
}

