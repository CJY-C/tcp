#pragma once

#define STATE_NUM 3
#define SOCKET_ERROR -1

#include <bitset>
#include <string>
#include <queue>

namespace Network {

struct NetworkPros
{
    static int Domain;
    static int Type;
    static int Protocol;

    NetworkPros() = default;
    ~NetworkPros() = default;
};

class TcpNode
{
public:
    TcpNode() = default;
    TcpNode(std::string name) : Name(name){}
    virtual ~TcpNode() = default;

    /* server and client */
    int CreateSocket();
    void Close();
    virtual int Send(const char* buf, unsigned int bufSize);
    //TODO: 最后一个参数的配置 
    virtual int Recv(char* buf, unsigned int bufSize);
    bool SetBlock(bool isBlock);

    /* server only */
    //TODO: 更多配置：监听的队列大小，ip地址
    virtual bool Bind(unsigned short port);
    virtual TcpNode Accept();

    /* client only */
    virtual bool Connect(std::string ip, unsigned short port, int timeout=1000);
    virtual bool Disconnect();
    virtual void TriggerConnect(std::string ip, unsigned short port);
    virtual void TriggerDisconnect();
    virtual void TriggerSend();
    virtual std::bitset<STATE_NUM> GetActiveEvent();
    virtual bool Sending();

public:
    /* attributes */
    std::string Name;
    int Socket = 0;
    unsigned short Port = 0;
    std::string IP;

    /* message queue */
    std::queue<std::string> m_StatusQueues;
    std::queue<std::string> m_MessageQueues;

    /* marks */
    bool Running;
    
};

}
