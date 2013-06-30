#include "sockmanager.h"
#include "global.h"

void CSockInfo::init() 
{
    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_port = htons(6666);
    m_serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_listen_fd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(m_listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    if (bind(m_listen_fd,(struct sockaddr *)&m_serv_addr,sizeof(m_serv_addr)) == -1)
    {
        cout << "bind error" << endl;
        close(m_listen_fd);
    }
    if (listen(m_listen_fd,5) == -1)
    {
        cout << "listen error" << endl;
        close(m_listen_fd);
    }

    m_len = sizeof(m_cli_addr);
    cout << "server init succeed" << endl;
}

int CSockInfo::accept_cli()
{
    int conn_fd = accept(m_listen_fd,(struct sockaddr *)&m_cli_addr,&m_len);
    cout << "port:" << ntohs(get_cli().sin_port) << ";current connect fd = " << conn_fd << ";connected num = " << g_sock_name.size() + 1 << endl;
    CLog::log_current_time();
    g_sock_name[conn_fd] = "";
    FD_SET(conn_fd,&g_all_set);
    g_max_fd = g_max_fd < conn_fd? conn_fd:g_max_fd;
    return conn_fd;
}

