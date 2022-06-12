/**
 * @file TCPTCPThread.h
 * @brief 线程抽象类，派生出客户端、服务端线程类
 */
#pragma once

#include "TCPNode.h"

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"

#include <string>
#include <thread>
#include <unordered_map>

namespace Network{

/**
 * @brief 客户端以及服务端线程管理类的抽象
 */
class TCPThread
{
public:
    /**
      * @brief 构造函数，初始化UI，启动tcp节点线程
      */
    TCPThread(std::string name="未命名") 
        : Name(name)
        , StatusLog(ICON_FK_SPINNER "状态栏")
        , MessageLog(ICON_FK_ENVELOPE "消息接收栏") 
    {
        m_TCPThread = std::thread(&TCPThread::Main, this);
    }

    /**
      * @brief 析构函数中等待主线程结束 
      */
    virtual ~TCPThread()
    {
        Node->Close();
        if (Node)
            delete Node;
        m_TCPThread.join();
    }

    /**
      * @brief  接口->线程主函数
      */
    virtual void Main() = 0;

    /**
      * @brief  接口->UI渲染，运行在主线程中
      * @param[open] 配置此UI中的组件的可见性
      */
    virtual void UIRenderer(bool *open) = 0;

public:
    std::string Name; /**< 线程名称 */

    TCPNode* Node=nullptr; /**< 线程中维护的tcp节点 */

    /* UI */
    ImLog MessageLog; /**< 消息栏UI */
    ImLog StatusLog; /**< 状态栏UI */
    std::string StatusInfo; /**< 保存tcp节点运行状态的信息 */

private:
    std::thread m_TCPThread; /**< tcp节点线程 */

};

}

