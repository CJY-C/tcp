#include "Client.h"
#include "Server.h"
#include "Network.h"

#include <string.h>
#include <thread>
#include <unistd.h>
#include <iostream>

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
        int last = info.tcpi_retransmits;
        if(getsockopt(fd,IPPROTO_TCP,TCP_INFO,(void*)&info,(socklen_t*)&length)==0)
        {
            if (last != info.tcpi_retransmits)
            {
                printf("tcpi_retransmits:%u\t\t\n",info.tcpi_retransmits);
                printf("tcpi_total_retrans:%u\t\t\n",info.tcpi_total_retrans);
            }
            printf("tcpi_rto:%u\t\t\n",info.tcpi_rto);
            printf("tcpi_ato:%u\t\t\n",info.tcpi_ato);

            printf("tcpi_rtt:%u\t\t\n",info.tcpi_rtt);
            printf("tcpi_last_data_send:%u\t\t\n",info.tcpi_last_data_sent);
            printf("tcpi_last_data_resv:%u\t\t\n",info.tcpi_last_data_recv);
            printf("tcpi_last_ack_send:%u\t\t\n",info.tcpi_last_ack_sent);
            printf("tcpi_last_ack_resv:%u\t\t\n",info.tcpi_last_ack_recv);
        }
        sleep(1);
    }
}
    Client::Client()
    {
        Init();
        m_test = std::thread(read_cwnd, m_Sockfd);
    }
    void Client::Init()
    {
        //第1步：创建客户端的socket。
        if((m_Sockfd = socket(Network::Domain, Network::Type, Network::Protocol)) == -1)
        {
            perror("socket");
            return;
        }
    }

    void Client::Connect(const Server *const server)
    {
        /* test sending thread */
        m_ConnectingThread = std::thread(&Client::Connecting, this, server);
    }

    void Client::Connecting(const Server* const server)
    {
        /* 待修改 */
        if (!m_Running)
        {
            Init();
            m_Running = true;
        }
        //第2步：向服务器发起连接请求。
        Sending = true;
        struct hostent* h;
        if((h = gethostbyname(server->HostName.c_str())) == 0)
        {
            printf("gethostbyname failed.\n");
            close(m_Sockfd);
            return;
        }
        
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(5005);
        memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

        if(connect(m_Sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        {
            perror("connect");
            close(m_Sockfd);
            return;
        }
        Sending = false;
    }

    void Client::Send()
    {
        if (m_ConnectingThread.joinable())
            m_ConnectingThread.join();
        if (m_SendingThread.joinable())
            m_SendingThread.join();
        m_SendingThread = std::thread(&Client::Sends, this);
    }

    void Client::Sends()
    {
        Sending = true;
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        //第3步：与服务端通信，发送一个报文后等待回复，然后再发下一个报文。
        for(int ii=0; ii<3; ii++)
        {
            int iret;
            sprintf(buffer, "Message id: %03d。", ii+1);
            
            if((iret = send(m_Sockfd, buffer, strlen(buffer), 0)) <= 0)
            {
                perror("send");
                break;
            }
            std::cout << "[Client]len: " << iret << std::endl;
            printf("[Client]发送：%s\n", buffer);

            memset(buffer, 0, sizeof(buffer));
            
            if((iret = recv(m_Sockfd, buffer, sizeof(buffer), 0)) <= 0)
            {
                printf("iret = %d\n", iret);
                break;
            }
            printf("[Client]接收：%s\n", buffer);
        }
        Sending = false;
    }

    void Client::Shutdown()
    {
        //第4步：关闭socket，释放资源。
        if (m_Running)
        {
            m_SendingThread.join();
            std::cout << "客户端关闭" << std::endl;
            m_Running = false;
            close(m_Sockfd);
        }
        else
            std::cout << "客户端已关闭" << std::endl;
    }
}
