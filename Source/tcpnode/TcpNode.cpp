#include "TcpNode.h"

#include <iostream>

#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Network {
int NetworkPros::Domain = AF_INET;
int NetworkPros::Type = SOCK_STREAM;
int NetworkPros::Protocol = INADDR_ANY;

/* server and client */

int TcpNode::CreateSocket()
{
    //TODO: 根据Network属性创建socket
    if (Socket <= 0)
    {
        Socket = socket(NetworkPros::Domain, NetworkPros::Type, NetworkPros::Protocol);

        if (Socket == SOCKET_ERROR)
        {
            //TODO: 将错误信息保存到状态队列中
            Close();
            std::cout << "创建socket失败！" << std::endl;
        }
        //TODO: 将创建成功信息保存到状态队列中
        std::cout << "创建socket成功！" << std::endl;

    }
    return Socket;
}

void TcpNode::Close()
{
    //TODO: Running置位false后，会调用。关闭socket，释放资源
    if (Socket <= 0)
        return;
    std::cout << "关闭sokect释放资源！" << std::endl;
    close(Socket);
    Socket = 0;
}

int TcpNode::Send(const char *buf, unsigned int bufSize)
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
    return sendedSize;
}

int TcpNode::Recv(char *buf, unsigned int bufSize)
{
    return recv(Socket, buf, bufSize, 0);
}

bool TcpNode::SetBlock(bool isBlock)
{
    if (Socket <= 0)
        return false;
    int flags = fcntl(Socket, F_GETFL, 0);
    if (flags < 0)
        return false;
    if (isBlock)
    {
        flags = flags&~O_NONBLOCK;
    }
    else
    {
        flags = flags|O_NONBLOCK;
    }
    if (fcntl(Socket, F_SETFL, flags) != 0)
        return false;
    return true;
}
/* server and client */


/* server only */

bool TcpNode::Bind(unsigned short port)
{
    CreateSocket();
    Port = port;
    struct sockaddr_in saddr;
    saddr.sin_family = NetworkPros::Domain;
    saddr.sin_port = htons(Port);
    /* saddr.sin_addr.s_addr = inet_addr("127.0.0.1"); */
    saddr.sin_addr.s_addr = htonl(0);

    if (::bind(Socket, (struct sockaddr*)&saddr, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        //TODO: 绑定错误保存到状态队列
        std::cout << "绑定端口失败!" << std::endl;
        Close();
        return false;
    }

    //TODO: 绑定成功保存到状态队列
    /* std::cout << "绑定端口成功！" << std::endl; */

    if (listen(Socket, 10) == SOCKET_ERROR)
    {
        std::cout << "监听失败！" << std::endl;
        Close();
    }
    return true;
}

TcpNode TcpNode::Accept()
{
    TcpNode clientNode;
    sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    int client = accept(Socket, (struct sockaddr*)&caddr, &len);

    if (client == SOCKET_ERROR)
    {
        //TODO: 队列错误
        /* std::cout << "连接错误" << std::endl; */
        return clientNode;
    }

    //TODO: 队列客户端信息，成功信息
    /* std::cout << "建立连接成功" << std::endl; */
    clientNode.Socket = client;
    clientNode.IP = inet_ntoa(caddr.sin_addr);
    clientNode.Port = ntohs(caddr.sin_port);
    /* std::cout << "客户端IP: " << clientNode.IP << std::endl; */
    /* std::cout << "客户端Port: " << clientNode.Port << std::endl; */

    return clientNode;
}
/* server only */

/* client only */
bool TcpNode::Connect(std::string ip, unsigned short port, int timeout)
{
    return false;
}
bool TcpNode::Disconnect()
{
    return false;
}
void TcpNode::TriggerConnect(std::string ip, unsigned short port)
{
}
void TcpNode::TriggerDisconnect()
{
}
void TcpNode::TriggerSend()
{
}
std::bitset<STATE_NUM> TcpNode::GetActiveEvent()
{
    return 0;
}
bool TcpNode::Sending()
{
    return false;
}
/* client only */
}
