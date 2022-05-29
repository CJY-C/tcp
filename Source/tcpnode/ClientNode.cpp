#include "ClientNode.h"
#include "TcpNode.h"

#include <bits/types/struct_timeval.h>
#include <iostream>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

namespace Network {


/* override */
/* void ClientNode::TriggerConnect(std::string ip, unsigned short port) */ 
/* { */
/*     m_ServerIP = ip; */
/*     m_ServerPort = port; */

/*     //TODO: 将下面测试语句替换为 独热码设置 连接位 */
/*     if (!Connect(3000)) */
/*     { */
/*         Close(); */
/*         return; */
/*     } */
/*     printf("连接服务端成功！\n"); */
/* } */

bool ClientNode::Connect(std::string ip, unsigned short port, int timeout)
{
    m_ServerIP = ip;
    m_ServerPort = port;

    CreateSocket();


    struct hostent* server;
    server = gethostbyname(m_ServerIP.c_str());
    if(server == 0)
    {
        printf("读取服务端IP地址失败！\n");
        Close();
        return false;
    }
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = NetworkPros::Domain;
    servaddr.sin_port = htons(m_ServerPort);
    memcpy(&servaddr.sin_addr, server->h_addr, server->h_length);

    SetBlock(false);
    fd_set readSet;
    fd_set writeSet;
    if(connect(Socket, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) != 0)
    {
        FD_ZERO(&readSet);
        FD_ZERO(&writeSet);
        FD_SET(Socket, &readSet);
        FD_SET(Socket, &writeSet);
        timeval tm;
        tm.tv_sec = 0;
        tm.tv_usec = timeout * 1000;
        int num = select(Socket + 1, &readSet, &writeSet, 0, &tm);
        //TODO: 超时处理，还有待思考
        printf("num :%d\n", num);
        printf("read: %d, write: %d\n", FD_ISSET(Socket, &readSet), FD_ISSET(Socket, &writeSet));
        if (num <= 0 || (FD_ISSET(Socket, &readSet) && FD_ISSET(Socket, &writeSet))) 
        {
            printf("连接服务端失败或超时！:%s\n", strerror(errno));
            Close();
            return false;
        }
    }
    SetBlock(true);
    printf("连接服务端成功！\n");

    struct sockaddr_in saddr;
    socklen_t saddr_len = sizeof(saddr);
    getsockname(Socket, (struct sockaddr*)&saddr, &saddr_len);
    Port = saddr.sin_port;
    std::cout << "Port: " << Port << std::endl;

    return true;
}

void ClientNode::TriggerDisconnect() 
{
    Close();
}

int ClientNode::Send(const char* buf, unsigned int bufSize)
{
    //TODO: 变量名修改 
    int sendedSize = 0;
    while (sendedSize != bufSize)
    {
        int len = send(Socket, buf + sendedSize, bufSize - sendedSize, 0);
        if (len <= 0)
            break;
        sendedSize += len;
    }
    /* m_ActiveEvent &= 0; */
    m_Sending = false;
    return sendedSize;
}

void ClientNode::TriggerSend() 
{
    m_Sending = true;
    /* m_ActiveEvent |= 1; */
    /* Send("123", 4); */
    /* std::cout << "将事件置位" << m_ActiveEvent.to_string() << std::endl; */
}

std::bitset<STATE_NUM> ClientNode::GetActiveEvent() 
{
    return 0;
}

bool ClientNode::Sending() 
{
    return m_Sending;
}


}
