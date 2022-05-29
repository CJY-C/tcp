#pragma once

#include <bits/stdint-uintn.h>
#include <string>
#include <thread>

namespace Network{
class Server
{
public:
    Server(const char* hostName, uint16_t port);
    ~Server() = default;

    /* main */
    void Listening();
    void Shutdown();

private:
    /* init */
    void Init();

public:
    std::string HostName;
    uint16_t Port;
    
private:
    std::thread m_ListeningThread;
    int m_Listenfd = 0;
    int m_Clientfd = 0;
    bool m_Listening = true;
};
}
