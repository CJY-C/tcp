/**
 * * @file ClientThread.h
 * * @brief 客户端线程管理类
 */
#pragma once

#include "TCPThread.h"
#include <thread>
#include <sys/epoll.h>

namespace Network {

/**
  * @brief  客户端线程管理类
  * @details 客户端的UI界面的绘制，以及客户端线程的处理流程的实现
  */
class ClientThread
: public TCPThread
{
public:
    ClientThread(std::string name);
    ~ClientThread() = default;

    /* override */
    /**
      * @brief  根据设计的流程图，完成客户端的处理逻辑
      */
    void Main() override;

    /**
      * @brief  实现设计的客户端界面
      * @param[open] 改变此界面组件的可见状态
      */
    void UIRenderer(bool *open) override;

private:
    /**
      * @brief  客户端连接弹出菜单UI
      */
    void ConnectingServerUI();

    /**
      * @brief  客户端状态信息UI
      */
    void ClientStatusUI();

    /**
      * @brief  客户端发送消息UI
      */
    void ClientSendingUI();

    /**
      * @brief  监控套接字的丢包重发事件
      */
    void MonitorRetrans();

    /**
      * @brief  接收数据
      */
    void ReceiveMessage();

private:
    /* attributes */
    unsigned short port = 0; /**< 记录上次输入的端口号 */
    std::string m_msg = ""; /**< 记录待发送的消息 */
    char* m_CRCCodeNames[6]; /**< 保存六种CRC多项式的名称 */
    int m_CurrentCode = 0;

    /* epoll */
    int m_epfd;
    epoll_event m_Event;
    epoll_event m_Events[20];

    /* marks */
    bool m_ConfigServer = false; /**< 是否打开连接服务器菜单栏 */
    bool m_Connected = false; /**< 判断是否与服务端连接 */
    bool m_SendEvent = false; /**< 发送消息事件的触发标记 */

    /* thread */
    std::thread m_MonitorThread; /**< 监控套接字重发状态的线程 */
};

}
