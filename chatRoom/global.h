#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <iostream>
#include <string>
#include <map>
#include <stdio.h>
#include <event.h>
#include "sockmanager.h"

using namespace std;

extern CSockInfo        g_sock_info;
extern map<string,int>  g_name_sock;
extern map<int,string>  g_sock_name;
extern fd_set           g_all_set;
extern int              g_max_fd;

class CLog
{
    public:
        inline static void log_current_time() 
        {
            time_t ticks;
            time(&ticks);
            cout << "time:" << ctime(&ticks);
        }
};

enum packet_type 
{
    NAME = 1,
    HEART,
    ALL,
    PRIVATE,
    EXCEPTION
};

struct net_packet_head 
{
    int body_size;
    packet_type m_packet_type;
};

struct net_packet_body 
{
    char receiver[4];
    char message[20];
};

struct net_packet
{
    net_packet_head head;
    net_packet_body body;
};

#endif
