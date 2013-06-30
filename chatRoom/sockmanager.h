#ifndef _SOCK_MANAGER_H_
#define _SOCK_MANAGER_H_

#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;

class CSockInfo 
{
    private:
        socklen_t            m_len;
        int                  m_listen_fd;
        struct sockaddr_in   m_serv_addr;
        struct sockaddr_in   m_cli_addr;

    public:
        CSockInfo()
        {
            m_listen_fd = 0;
            m_len = 0;
        }

        inline int get_serv_sock()
        {
            return m_listen_fd;
        }

        inline sockaddr_in get_cli()
        {
            return m_cli_addr;
        }

        virtual void init();
        virtual int accept_cli();
};

#endif

