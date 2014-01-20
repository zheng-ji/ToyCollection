#ifndef COMMONS_H
#define COMMONS_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define	DEBUG		1

#define	PORTSERVER	8487
#define CONTROLPORT	PORTSERVER
#define DATAPORT	(PORTSERVER + 1)

enum TYPE
{
    REQU,
    DONE,
    INFO,
    TERM,
    DATA,
    EOT
};

#define	NP		0
#define	HP		1

#define	er(e, x)					    \
    do						            \
    {						            \
        perror("ERROR IN: " #e "\n");	\
        fprintf(stderr, "%d\n", x);		\
        exit(-1);				        \
    }						            \
while(0)

#define	LENBUFFER	504		// so as to make the whole packet well-rounded ( = 512 bytes)
struct packet
{
    short int conid;
    short int type;
    short int comid;
    short int datalen;
    char buffer[LENBUFFER];
};

void set0(struct packet*);
struct packet* ntohp(struct packet*);
struct packet* htonp(struct packet*);
void printpacket(struct packet*, int);

#define NCOMMANDS 4
enum COMMAND
{
    GET,
    PUT,
    CD,
    LS
};			// any change made here should also be 

#endif

