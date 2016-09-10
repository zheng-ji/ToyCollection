#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace std;

#define MAXLINE 5
#define OPEN_MAX 100
#define LISTENQ 20
#define SERV_PORT 5000
#define INFTIM 1000

void setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);
    if(opts<0)
    {
        perror("fcntl(sock,GETFL)");
        return;
    }
    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return;
    }
}

void CloseAndDisable(int sockid, epoll_event ee)
{
    close(sockid);
    ee.data.fd = -1;
}

int main()
{
    int i, maxi, listenfd, connfd, sockfd,epfd,nfds, portnumber;
    char line[MAXLINE];
    socklen_t clilen;

    portnumber = 5000;

    //声明epoll_event结构体的变量,ev用于注册事件,数组用于回传要处理的事件

    struct epoll_event ev,events[20];
    //生成用于处理accept的epoll专用的文件描述符

    epfd=epoll_create(256);
    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port=htons(portnumber);

    // bind and listen
    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LISTENQ);

    //设置与要处理的事件相关的文件描述符
    ev.data.fd=listenfd;
    //设置要处理的事件类型
    ev.events=EPOLLIN|EPOLLET;
    //ev.events=EPOLLIN;

    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);

    maxi = 0;

    int bOut = 0;
    for ( ; ; )
    {
        if (bOut == 1)
            break;
        //等待epoll事件的发生

        nfds=epoll_wait(epfd,events,20,-1);
        //处理所发生的所有事件
        cout << "\nepoll_wait returns\n";

        for(i=0;i<nfds;++i)
        {
            if(events[i].data.fd==listenfd)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            {
                connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
                if(connfd<0){
                    perror("connfd<0");
                    return (1);
                }


                char *str = inet_ntoa(clientaddr.sin_addr);
                cout << "accapt a connection from " << str << endl;
                //设置用于读操作的文件描述符

                setnonblocking(connfd);
                ev.data.fd=connfd;
                //设置用于注测的读操作事件

                ev.events=EPOLLIN | EPOLLET;
                //ev.events=EPOLLIN;

                //注册ev
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }
            else if(events[i].events & EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
            {
                cout << "EPOLLIN" << endl;
                if ( (sockfd = events[i].data.fd) < 0)
                    continue;

                char * head = line;
                int recvNum = 0;
                int count = 0;
                bool bReadOk = false;
                while(1)
                {
                    // 确保sockfd是nonblocking的
                    recvNum = recv(sockfd, head + count, MAXLINE, 0);
                    if(recvNum < 0)
                    {
                        if(errno == EAGAIN)
                        {
                            // 由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可读
                            // 在这里就当作是该次事件已处理处.
                            bReadOk = true;
                            break;
                        }
                        else if (errno == ECONNRESET)
                        {
                            // 对方发送了RST
                            CloseAndDisable(sockfd, events[i]);
                            cout << "counterpart send out RST\n";
                            break;
                        }
                        else if (errno == EINTR)
                        {
                            // 被信号中断
                            continue;
                        }
                        else
                        {
                            //其他不可弥补的错误
                            CloseAndDisable(sockfd, events[i]);
                            cout << "unrecovable error\n";
                            break;
                        }
                    }
                    else if( recvNum == 0)
                    {
                        // 这里表示对端的socket已正常关闭.发送过FIN了。
                        CloseAndDisable(sockfd, events[i]);
                        cout << "counterpart has shut off\n";
                        break;
                    }

                    // recvNum > 0
                    count += recvNum;
                    if ( recvNum == MAXLINE)
                    {
                        continue;   // 需要再次读取
                    }
                    else // 0 < recvNum < MAXLINE
                    {
                        // 安全读完
                        bReadOk = true;
                        break; // 退出while(1),表示已经全部读完数据
                    }
                }

                if (bReadOk == true)
                {
                    // 安全读完了数据
                    line[count] = '\0';

                    cout << "we have read from the client : " << line;
                    //设置用于写操作的文件描述符

                    ev.data.fd=sockfd;
                    //设置用于注测的写操作事件

                    ev.events = EPOLLOUT | EPOLLET;
                    //修改sockfd上要处理的事件为EPOLLOUT

                    epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                }
            }
            else if(events[i].events & EPOLLOUT) // 如果有数据发送
            {
                const char str[] = "hello from epoll : this is a long string which may be cut by the net\n";
                memcpy(line, str, sizeof(str));
                cout << "Write " << line << endl;
                sockfd = events[i].data.fd;

                bool bWritten = false;
                int writenLen = 0;
                int count = 0;
                char * head = line;
                while(1)
                {
                    // 确保sockfd是非阻塞的
                    writenLen = send(sockfd, head + count, MAXLINE, 0);
                    if (writenLen == -1)
                    {
                        if (errno == EAGAIN)
                        {
                            // 对于nonblocking 的socket而言，这里说明了已经全部发送成功了
                            bWritten = true;
                            break;
                        }
                        else if(errno == ECONNRESET)
                        {
                            // 对端重置,对方发送了RST
                            CloseAndDisable(sockfd, events[i]);
                            cout << "counterpart send out RST\n";
                            break;
                        }
                        else if (errno == EINTR)
                        {
                            // 被信号中断
                            continue;
                        }
                        else
                        {
                            // 其他错误
                        }
                    }

                    if (writenLen == 0)
                    {
                        // 这里表示对端的socket已正常关闭.
                        CloseAndDisable(sockfd, events[i]);
                        cout << "counterpart has shut off\n";
                        break;
                    }

                    // 以下的情况是writenLen > 0
                    count += writenLen;
                    if (writenLen == MAXLINE)
                    {
                        // 可能还没有写完
                        continue;
                    }
                    else // 0 < writenLen < MAXLINE
                    {
                        // 已经写完了
                        bWritten = true;
                        break; // 退出while(1)
                    }
                }

                if (bWritten == true)
                {
                    //设置用于读操作的文件描述符
                    ev.data.fd=sockfd;

                    //设置用于注测的读操作事件
                    ev.events=EPOLLIN | EPOLLET;

                    epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);
                }
            }
        }
    }
    return 0;
}
