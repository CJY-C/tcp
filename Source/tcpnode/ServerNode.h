/**
 * @file ServerNode.h
 * @brief 服务端节点
 * @details 实现接口Bind(port)、Accept()
 */
#pragma once 

#include "TCPNode.h"

namespace Network {

/**
 * @brief 服务端节点
 */
class ServerNode
: public TCPNode
{
public:
    ServerNode() = default;
    ServerNode(std::string name) : TCPNode(name){}
    ~ServerNode() = default;

    /* overrides */

    /**
      * @brief  绑定到指定端口
      * @param[port] 设置的端口号
      * @return TCPNodeRetrun::SUCCESS 绑定成功
      * @return TCPNodeRetrun::ERROR 绑定过程中出错
      */
    bool Bind(unsigned short port) override;

    /**
      * @brief  开启监听
      * @param[port] 设置的监听的队列长度
      * @return TCPNodeRetrun::SUCCESS 设置监听成功
      * @return TCPNodeRetrun::ERROR 设置监听出错
      */
    bool Listen(unsigned int len=3) override;

    /**
      * @brief  接受客户端的连接
      * @return 返回Tcp节点，通过其Socket成员变量是否为0判断是否建立连接成功
      */
    TCPNode Accept() override;
};

}
