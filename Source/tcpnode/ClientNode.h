/**
 * @file ClientNode.h
 * @brief 客户端节点
 * @details 实现接口Connect、Disconnect，重载Send
 * @todo 错误信息保存
 */
#pragma once

#include "TCPNode.h"

namespace Network {

/**
  * @brief 客户端节点
  */
class ClientNode
    : public TCPNode
{
public:
    ClientNode() = default;
    ClientNode(std::string name) : TCPNode(name){}
    ~ClientNode() = default;

    /* override */
    //FIXME: 解决默认参数问题
    /**
      * @brief     连接到指定服务端
      * @param[ip] 服务端地址
      * @param[port] 服务端端口号
      * @param[timeout] 连接超时等待时间
      * @return    TCPNodeRetrun::SUCCESS  连接成功
      * @return    TCPNodeRetrun::ERROR    连接出错，保存错误信息
      */
    bool Connect(std::string ip, unsigned short port, int timeout=7000) override;

    /**
      * @brief     主动断开连接
      * @return    TCPNodeRetrun::SUCCESS  断开连接成功
      * @return    TCPNodeRetrun::ERROR    断开连接出错，保存错误信息
      */
    bool Disconnect() override;


private:
    /* server info */
    std::string m_ServerIP; /**<服务端IP地址 */
    unsigned short m_ServerPort; /**<服务端端口号 */
};

}
