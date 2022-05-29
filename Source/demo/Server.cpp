#include "Server.h"

#include "Network.h"
#include <string.h>
#include <iostream>
#include <poll.h>
#include <sys/select.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>

namespace Network{
static void read_cwnd(int fd)
{
    struct tcp_info info;
    int length=sizeof(struct tcp_info);
    while (true)
    {
        if(getsockopt(fd,IPPROTO_TCP,TCP_INFO,(void*)&info,(socklen_t*)&length)==0)
        {
                printf("tcpi_retransmits:%u\t\t\n",info.tcpi_retransmits);
        }
        sleep(1);
    }
}

    Server::Server(const char* hostName, uint16_t port)
        : HostName(hostName)
        , Port(port)
    {
        Init();
    }

    void Server::Init()
    {
        // 第1步：创建服务端的socket。
        if ((m_Listenfd = socket(Network::Domain, Network::Type, Network::Protocol)) == -1)
        {
            perror("socket");
            return;
        }
        //第2步：把服务端用于通信的地址和端口绑定到socket上。
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = Network::Domain;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(Port);

        if(bind(m_Listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        {
            perror("bind");
            close(m_Listenfd);
            return;
        }

        m_ListeningThread = std::thread(&Server::Listening, this);
    }

    void Server::Listening()
    {
        //第3步，把socket设置为监听模式。
        if(listen(m_Listenfd, 5) != 0)
        {
            perror("listen");
            close(m_Listenfd);
            return;
        }
        int socklen = sizeof(struct sockaddr_in);
        struct sockaddr_in clientaddr;

        //第4步：接受客户端的连接。
        fd_set read_fds;

        struct timeval timeout;
        timeout.tv_sec = 1;  // 1s timeout
        timeout.tv_usec = 0;

        /* std::thread read_cwn(read_cwnd, m_Listenfd); */
        int select_status;
        while (m_Listening) {
            /* 重点 */
            FD_ZERO(&read_fds);
            FD_SET(m_Listenfd, &read_fds);

            select_status = select(m_Listenfd+1, &read_fds, NULL, NULL, &timeout);
            if (select_status == -1) {
                std::cout << "[select] error occcurs!!" << std::endl;
            } else if (select_status > 0) {
                std::cout << "select_status: " << select_status << std::endl;
                m_Clientfd = accept(m_Listenfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
                printf("[Server] Client(%s) has connected\n", inet_ntoa(clientaddr.sin_addr));
                //第5步：与客户端通信，接受客户端发过来的报文后，回复ok。
                while(m_Listening)
                {
                    if (FD_ISSET(m_Listenfd, &read_fds))
                    {
                        char buffer[1024];
                        int iret;
                        memset(buffer, 0, sizeof(buffer));
                        if((iret = recv(m_Clientfd, buffer, sizeof(buffer), 0)) <= 0 )
                        {
                            printf("iret = %d\n", iret);
                            break;
                        }
                        printf("[Server]接收：%s\n", buffer);

                        strcpy(buffer, "ok");
                        if((iret = send(m_Clientfd, buffer, strlen(buffer), 0)) <= 0)
                        {
                            perror("send");
                            break;
                        }
                        std::cout << "[Server]len: " << iret << std::endl;
                        printf("[Server]发送：%s\n",buffer);

                    }
                }
            }
            /* std::cout << "scanning" << std::endl; */
            // otherwise (i.e. select_status==0) timeout, continue
        }

    }

    void Server::Shutdown()
    {
        if (m_Listening)
        {
            std::cout << "服务端关闭" << std::endl;
            m_Listening = false;
            m_ListeningThread.join();

            //第6步：关闭socket，释放资源。
            close(m_Listenfd);
            close(m_Clientfd);
        }
        else
            std::cout << "服务端已关闭" << std::endl;
    }
}
