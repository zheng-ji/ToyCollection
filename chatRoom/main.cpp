#include <iostream>
#include "sockmanager.h"
#include "chatmanager.h"
#include "global.h"
//#include <event.h>

using namespace std;

CSockInfo           g_sock_info;
map<string,int>     g_name_sock;
map<int,string>     g_sock_name;
fd_set              g_all_set;
int                 g_max_fd(0);
event               g_serv_ev;

int main(int argc, char* argv[])
{
	struct timeval      ticks;
	CChatManager      chat_manager;
	fd_set              cur_set;

    event_init();      //libevent: event_init

	g_sock_info.init();
    //event_set(g_serv_ev, g_sock_info.get_serv_sock(), EV_READ | EV_PERSIST, serv_accept, NULL);
	g_max_fd = g_sock_info.get_serv_sock();
	ticks.tv_sec = 5;
	ticks.tv_usec = 0;
	FD_ZERO(&g_all_set);
	FD_SET(g_sock_info.get_serv_sock(), &g_all_set); 

	while(1)
	{
		cur_set = g_all_set;

		switch(select(g_max_fd+1, &cur_set, NULL, NULL, &ticks))
		{
			case -1:
				cout << "select error!" << endl;
			    return 0;	
			case 0:
				break;
			default:
				if(g_sock_name.size() > 0)
				{
					for(map<int,string>::iterator it = g_sock_name.begin(); it != g_sock_name.end(); it++)
					{
						if(FD_ISSET((*it).first, &cur_set))
						{
							chat_manager.send_message((*it).first);
						}
					}
				}
				if(FD_ISSET(g_sock_info.get_serv_sock(), &cur_set))
				{
					g_sock_info.accept_cli();
				}
		}
	}

	return 0;
}
