#pragma once

#include "Server.h"

namespace Network{
    class Client
    {
    public:
        Client();
        ~Client() = default;

        /* main */
        void Connect(const Server* const server);
        void Send();
        void Shutdown();

    private:
        /* init */
        void Init();
        void Connecting(const Server* const server);
        void Sends();

    public:
        bool Sending = false;

    private:
        std::thread m_SendingThread;
        std::thread m_test;
        std::thread m_ConnectingThread;
        bool m_Running = true;
        int m_Sockfd = 0;
        
    };
}
