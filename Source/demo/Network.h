#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace Network {
class Network
{
public:
    static int Domain;
    static int Type;
    static int Protocol;
public:
    Network() = default;
    ~Network() = default;
};
}
