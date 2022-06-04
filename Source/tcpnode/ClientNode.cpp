#include "ClientNode.h"

#include <string.h>
#include <bits/types/struct_timeval.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

namespace Network {


/* override */

bool ClientNode::Connect(std::string ip, unsigned short port, int timeout)
{
    m_ServerIP = ip;
    m_ServerPort = port;

    CreateSocket();


    struct hostent* server;
    server = gethostbyname(m_ServerIP.c_str());
    if(server == 0)
    {
        strcpy(ErrorInfo, strerror(errno));
        Close();
        return TCPNodeRetrun::ERROR;
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
        //TODO: 这边阻塞需要调整
        tm.tv_usec = timeout * 1000;
        int num = select(Socket + 1, &readSet, &writeSet, 0, &tm);
        //TODO: 超时处理，还有待思考
        /* printf("num :%d\n", num); */
        /* printf("read: %d, write: %d\n", FD_ISSET(Socket, &readSet), FD_ISSET(Socket, &writeSet)); */
        if (num <= 0 || (FD_ISSET(Socket, &readSet) && FD_ISSET(Socket, &writeSet))) 
        {
            strcpy(ErrorInfo, strerror(errno));
            Close();
            return TCPNodeRetrun::ERROR;
        }
    }
    /* SetBlock(true); */

    struct sockaddr_in saddr;
    socklen_t saddr_len = sizeof(saddr);
    getsockname(Socket, (struct sockaddr*)&saddr, &saddr_len);
    Port = saddr.sin_port;

    return TCPNodeRetrun::SUCCESS;
}

bool ClientNode::Disconnect()
{
    Close();
    return TCPNodeRetrun::SUCCESS;
}

}
