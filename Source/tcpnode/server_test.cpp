#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <sys/epoll.h>
#include "ServerNode.h"

int main(int argc, char* argv[])
{
    Network::ServerNode node("Test server");
    std::cout << "tcp node :" << node.Name << std::endl;
    if (argc < 2)
    {
        std::cout << "usage: ./server_test <port>" << std::endl;
        return 0;
    }
    unsigned short port;
    port = atoi(argv[1]);

    node.CreateSocket();
    node.Bind(port);

    std::cout << "等待客户端连接..." << std::endl;
    char buf[521];
    memset(buf, 0, sizeof(buf));
    int epfd = epoll_create(256);

    epoll_event ev;
    ev.data.fd = node.Socket;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, node.Socket, &ev);

    epoll_event events[20];

    node.SetBlock(false);
    while (true)
    {
        int cnt = epoll_wait(epfd, events, 20, 500);
        if (cnt <= 0)
            continue;
        for (int i = 0; i < cnt; i++) {
            if (events[i].data.fd == node.Socket)
            {
                while (true)
                {
                    Network::TcpNode client = node.Accept();
                    if (client.Socket <= 0) break;
                    ev.data.fd = client.Socket;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client.Socket, &ev);
                }
            }
            else
            {
                Network::TcpNode client;
                client.Socket = events[i].data.fd;
                if (client.Recv(buf, sizeof(buf)) <= 0)
                {
                    std::cout << "断开连接：" << buf << std::endl;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.Socket, &ev);
                }
                else
                {
                    std::cout << "接收消息：" << buf << std::endl;
                    client.Send("ok", 3);
                }
            }
        }
    }
    node.Close();
    return 0;
}
