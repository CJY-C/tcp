#pragma once

#include "Thread.h"

namespace Network {

class ClientThread
: public Thread
{
public:
    ClientThread(std::string name)
        :Thread(name)
    {
    }
    ~ClientThread() = default;

    /* override */
    void Main() override;
    void UIRenderer(bool *open) override;

private:
    void ConnectingServerUI();

private:
    bool m_configServer=false;

    std::string m_msg = "";
    unsigned short port;
};

}
