#pragma once

#include "TcpNode.h"
#include <bitset>
#include <string>

namespace Network {

class ClientNode
    : public TcpNode
{
public:
    ClientNode() = default;
    ClientNode(std::string name) : TcpNode(name){}
    ~ClientNode() = default;

    /* override */
    bool Connect(std::string ip, unsigned short port, int timeout=1000) override;
    /* void TriggerConnect(std::string ip, unsigned short port) override; */
    void TriggerDisconnect() override;
    int Send(const char* buf, unsigned int bufSize) override;
    void TriggerSend() override;
    std::bitset<STATE_NUM> GetActiveEvent() override;
    bool Sending() override;

private:
    /* server info */
    std::string m_ServerIP;
    unsigned short m_ServerPort;

    /* marks */
    std::bitset<STATE_NUM> m_ActiveEvent=0;
    bool m_Sending = false;
};

}
