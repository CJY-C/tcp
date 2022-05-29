#pragma once

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "TcpNode.h"
#include <string>
#include <iostream>
#include <thread>
#include <unordered_map>

namespace Network{
class Thread
{
public:
    Thread(std::string name="未命名") 
        : Name(name)
        , StatusLog(ICON_FK_SPINNER "状态栏")
        , MessageLog(ICON_FK_ENVELOPE "消息接收栏") 
    {
        m_Thread = std::thread(&Thread::Main, this);
    }
    virtual ~Thread()
    {
        m_Thread.join();
    }

    /* main -> tcp node thread */
    virtual void Main() = 0;

    /* ui -> main thread */
    virtual void UIRenderer(bool *open) = 0;

public:
    std::string Name;
    std::string StatusInfo;

    TcpNode* tcpNode=nullptr;
    /* UI */
    ImLog StatusLog;
    ImLog MessageLog;

private:
    std::thread m_Thread;

};

class ServerThread
: public Thread
{
public:
    ServerThread(std::string name)
        :Thread(name)
    {
    }
    ~ServerThread() = default;

    /* override */
    void Main() override;
    void UIRenderer(bool *open) override;

private:
    std::unordered_map<int, TcpNode> m_ConnectedClients;
};

}

