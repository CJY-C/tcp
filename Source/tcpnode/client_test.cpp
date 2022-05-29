#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include "ClientNode.h"

#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>

int main(int argc, char* argv[])
{
    Network::ClientNode node("Test client");
    std::cout << "tcp node :" << node.Name << std::endl;
    if (argc < 2)
    {
        std::cout << "usage: ./client_test <port>" << std::endl;
        return 0;
    }
    unsigned short port;
    port = atoi(argv[1]);

    /* node.TriggerConnect("127.0.0.1", port); */
    bool success = node.Connect("127.0.0.1", port);

    if (success)
    {
        while (success)
        {
            int size = node.Send("nihao", 6);
            std::cout << "send size: " << size << std::endl;
            char buf[512];
            node.Recv(buf, sizeof(buf));
            std::cout << "收到服务端回复: " << buf << std::endl;
            int quit = 0;
            std::cin >> quit;
            if (quit)
                break;
        }
        shutdown(node.Socket, SHUT_WR);
    }

    node.Close();

    return 0;
}
