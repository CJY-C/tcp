/**
 * @file ServerThread.h
 * @brief 服务器线程管理类
 */
#pragma once

#include "TCPThread.h"

#include <sys/epoll.h>

namespace Network {

/**
  * @brief  服务端线程管理类
  * @details 服务端的UI界面的绘制，以及服务端线程的处理流程的实现
  */
class ServerThread
: public TCPThread
{
public:
    ServerThread(std::string name) :TCPThread(name) {}
    ~ServerThread();

    /* override */
    /**
      * @brief  根据设计的流程图，完成服务端的处理逻辑
      */
    void Main() override;

    /**
      * @brief  实现设计的服务端界面
      * @param[open] 改变此界面组件的可见状态
      */
    void UIRenderer(bool *open) override;

private:
    void ServerEvent();
    void ClientEvent();

private:
    std::unordered_map<int, TCPNode> m_ConnectedClients; /**< 保存已连接的客户端 */
    int m_epfd;
    epoll_event m_Event;
    epoll_event m_Events[20];
};

}


