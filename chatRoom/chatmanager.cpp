#include "chatmanager.h"
#include "global.h"
#include <string.h>

using namespace std;

void static recv_usr_name(int conn_fd,const char * name);

int static packet_write(int write_fd,string &body,packet_type type);

void static clear_conn_fd(int conn_fd,const char * name);


#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024

int read_count = 0;

int static packet_write(int write_fd,string &body,packet_type type)
{
    net_packet write_packet;
    write_packet.head.body_size = body.size() + 1;
    write_packet.head.m_packet_type = type;
    write(write_fd, (char*)&write_packet.head, sizeof(net_packet_head));
    int body_ret = write(write_fd,body.data(),body.size() + 1);
    return body_ret;
}

void static recv_usr_name (int conn_fd,const char * name)
{
    if (g_name_sock.find(name) == g_name_sock.end())
    {
        g_sock_name[conn_fd] = name;
        g_name_sock[name] = conn_fd;
        string msg = "please start to chat";
        packet_write(conn_fd,msg,ALL);
        cout << g_sock_name[conn_fd] << " enter chat room" << endl;
    }
}

void static clear_conn_fd (int conn_fd)
{
    g_sock_name.erase(conn_fd);
    close(conn_fd);
    FD_CLR(conn_fd,&g_all_set);
}
void CChatManager::send_message(int conn_fd)
{
    char buf_head[sizeof(net_packet_head)];
    memset(buf_head,0,sizeof(buf_head));
    int read_head_ret = read(conn_fd,buf_head,sizeof(buf_head));
    int read_body_ret = 0;

    if (read_head_ret > 0)
    {
        net_packet read_packet;
        read_packet.head = *(net_packet_head *)buf_head;
        char buf_body[read_packet.head.body_size];
        memset(buf_body,0,sizeof(buf_body));
        read_body_ret = read(conn_fd,buf_body,read_packet.head.body_size);
        if (read_body_ret > 0)
        {
            if (read_packet.head.m_packet_type == NAME)
            {
                recv_usr_name(conn_fd,buf_body);
            }
            else 
            {
                char receiver[11];
                char *msg_ptr;
                msg_ptr = strchr(buf_body,':');
                if (msg_ptr == NULL)
                {
                    string err_input_format = "input format ";
                    packet_write(conn_fd,err_input_format,EXCEPTION);
                    cout << "recv wrong format message" << endl;
                    return;
                }

                strncpy(receiver,buf_body,msg_ptr - buf_body);
                receiver[msg_ptr - buf_body] = '\0';
                string sender = g_sock_name[conn_fd];
                if (read_packet.head.m_packet_type == ALL)
                {
                    sender = sender + ":" + (msg_ptr + 1) + " (in a chat room)";
                    cout << sender << "count: " << read_count ++ << endl;
                    map<int,string> tmp(g_sock_name);
                    tmp.erase(conn_fd);
                    for (map<int,string>::iterator it = tmp.begin();it != tmp.end(); it++)
                    {
                        packet_write((*it).first,sender,ALL);
                    }
                }
                else if (read_packet.head.m_packet_type == HEART)
                {
                    string heart_beat = "heart:beat";
                    packet_write(conn_fd,heart_beat,HEART);
                }
                else if (read_packet.head.m_packet_type == PRIVATE)
                {
                    sender = sender + ": " + (msg_ptr + 1) + "(private)";
                    cout << sender << endl;
                    int write_fd = (*g_name_sock.find(receiver)).second;
                    if (write_fd != 0)
                    {
                        packet_write(write_fd,sender,PRIVATE);
                    }
                    else
                    {
                        string target_not_exist = "target not exist";
                        packet_write(conn_fd,target_not_exist,EXCEPTION);
                    }
                }
            }
        }
    } 
    if (read_head_ret <= 0 || read_body_ret <= 0)
    {
        cout << g_sock_name[conn_fd] << "offline" << endl;
        g_name_sock.erase(g_sock_name[conn_fd]);
        clear_conn_fd(conn_fd);
    }
}

