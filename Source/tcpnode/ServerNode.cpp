#include "ServerNode.h"

#include <cerrno>
#include <cstdio>
#include <cstring>

#include <netinet/in.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Network {


bool ServerNode::Bind(unsigned short port)
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
        strcpy(ErrorInfo, strerror(errno));
        Close();
        return TCPNodeRetrun::ERROR;
    }

    return TCPNodeRetrun::SUCCESS;
}

bool ServerNode::Listen(unsigned int len)
{
    if (listen(Socket, len) == SOCKET_ERROR)
    {
        strcpy(ErrorInfo, strerror(errno));
        Close();
        return TCPNodeRetrun::SUCCESS;
    }
    return TCPNodeRetrun::SUCCESS;
}

TCPNode ServerNode::Accept()
{
    TCPNode clientNode;
    sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    int client = accept(Socket, (struct sockaddr*)&caddr, &len);

    if (client == SOCKET_ERROR)
    {
        //XXX:这里出错，不能释放资源！！
        return clientNode;
    }

    //TODO: 校验&获取客户端信息(名称)

    clientNode.Socket = client;
    clientNode.IP = inet_ntoa(caddr.sin_addr);
    clientNode.Port = ntohs(caddr.sin_port);

    return clientNode;
}

}
