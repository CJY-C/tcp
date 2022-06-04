#include "TCPNode.h"

#include <iostream>
#include <cerrno>
#include <cstring>

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

static const char* GetProtocolName(){
    switch (NetworkPros::Type)
    {
        case SOCK_STREAM:
            return "TCP";
            break;
    }
    return "None";
}

/* server and client */

int TCPNode::CreateSocket()
{
    if (Socket <= 0)
    {
        Socket = socket(NetworkPros::Domain, NetworkPros::Type, NetworkPros::Protocol);

        if (Socket == SOCKET_ERROR)
        {
            strcpy(ErrorInfo, strerror(errno));
            Close();
        }
    }
    return Socket;
}

void TCPNode::Close()
{
    if (Socket <= 0)
        return;
    close(Socket);
    Socket = 0;
}

int TCPNode::Send(const void* data, unsigned int size)
{
    return send(Socket, data, size, 0);
}

int TCPNode::Recv(void* buf, unsigned int bufSize)
{
    return recv(Socket, buf, bufSize, 0);
}

bool TCPNode::SetBlock(bool isBlock)
{
    if (Socket <= 0)
        return TCPNodeRetrun::ERROR;

    int flags = fcntl(Socket, F_GETFL, 0);
    if (flags < 0)
        return TCPNodeRetrun::ERROR;

    if (isBlock)
        flags = flags&~O_NONBLOCK;
    else
        flags = flags|O_NONBLOCK;

    if (fcntl(Socket, F_SETFL, flags) != 0)
        return TCPNodeRetrun::ERROR;

    return TCPNodeRetrun::SUCCESS;
}
/* server and client */


/* server only */
bool TCPNode::Bind(unsigned short port)
{
    std::cout << "Bind未实现" << std::endl;
    return false;
}

bool TCPNode::Listen(unsigned int len)
{
    std::cout << "Listen未实现" << std::endl;
    return false;
}

TCPNode TCPNode::Accept()
{
    std::cout << "Accept未实现" << std::endl;
    return TCPNode();
}


/* client only */
bool TCPNode::Connect(std::string ip, unsigned short port, int timeout)
{
    std::cout << "Connect未实现" << std::endl;
    return false;
}

bool TCPNode::Disconnect()
{
    std::cout << "Disconnect未实现" << std::endl;
    return false;
}
}
