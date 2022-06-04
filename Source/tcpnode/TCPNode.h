/**
 * @file TCPNode.h
 * @brief 简单封装的Tcp节点类
 * @details 派生出服务端、客户端节点
 */
#pragma once

#define STATE_NUM 3
#define SOCKET_ERROR -1

#include <bitset>
#include <string>

namespace Network {


/**
 * @brief 表示TCP节点的返回信息
 */
enum TCPNodeRetrun 
{
    ERROR = 0, /**< 发生错误 */
    SUCCESS = 1 /**< 正常 */
};


/**
 * @brief 用于配置创建客户端或者服务端套接字时所需的参数
 */
struct NetworkPros
{
    static int Domain; /**< 指定通信域，选择将用于通信的协议族 */
    static int Type; /**< 指定套接字类型：TCP通常使用SOCK_STREAM */
    static int Protocol; /**< 通常只存在一个协议来支持特定的套接字类型，因此设置0即可 */

    NetworkPros() = default;
    ~NetworkPros() = default;
};

/**
 * @brief 实现Tcp通信中服务端与客户端共用功能的封装，可继承实现服务端与客户端各自独有的功能
 */
class TCPNode
{
public:
    TCPNode() = default;
    /**
      * @brief   构造函数
      * @param[name] Tcp节点的名字
      */
    TCPNode(std::string name) : Name(name){}
    virtual ~TCPNode() = default;

    /**
      * @brief   创建一个基于NetworkPros中对应配置的套接字
      * @return  所创建的套接字
      */
    int CreateSocket();

    /**
      * @brief  将对象中已创建的套接字关闭，并释放资源
      */
    void Close();

//    /**
//      * @brief  发送传入的字符串，并且保证全部发送成功
//      * @param[buf] 发送的字符串指针
//      * @param[len] 包含的字符数
//      * @return 已发送的字符数
//      */
//    virtual int Send(const char* buf, unsigned int len);

    /**
      * @brief  发送传入的数据，并且保证全部发送成功
      * @param[data] 发送的发送数据的指针
      * @param[size] 数据的大小
      * @return 已发送数据的大小
      */
    virtual int Send(const void* data, unsigned int size);

//    /**
//      * @brief  接收字符串消息，保存到传入的字符串数组中
//      * @param[buf] 写入接收消息的字符串数组
//      * @param[bufSize] 传入的字符串数组的最大容量
//      * @return 已接收的字符数
//      * @todo 错误处理，接收的长度大于传入字符串数组的最大容量
//      */
//    virtual int Recv(char* buf, unsigned int bufSize);

    /**
      * @brief  接收数据，保存到传入的指针中
      * @param[buf] 写入接收消息的指针
      * @param[bufSize] 传入的指针最大容量
      * @return 已接收的数据量
      * @todo 错误处理，接收的长度大于传入指针的最大容量
      */
    virtual int Recv(void* buf, unsigned int bufSize);

    /**
      * @brief     设置套接字的阻塞模式
      * @param[isBlock] 阻塞（true），非阻塞（false）
      * @return    TCPNodeRetrun::SUCCESS  设置成功
      * @return    TCPNodeRetrun::ERROR    设置失败
      */
    bool SetBlock(bool isBlock);

    /**
      * @brief  接口->绑定端口
      * @param[port] 设置的端口号
      */
    virtual bool Bind(unsigned short port);

    /**
      * @brief  接口->监听连接
      * @param[len] 监听队列的长度
      */
    virtual bool Listen(unsigned int len=3);

    /**
      * @brief  接口->接收连接
      * @return    TCPNode
      */
    virtual TCPNode Accept();

    /**
      * @brief  接口->连接
      * @return    bool
      */
    virtual bool Connect(std::string ip, unsigned short port, int timeout=3000);

    /**
      * @brief  接口->断开连接
      * @return    bool
      */
    virtual bool Disconnect();

public:
    /* attributes */
    std::string Name; /**< 节点名称 */
    char ErrorInfo[256] = {0}; /**< 节点成员函数调用过程中产生的错误信息 */
    int Socket = 0; /**< 套接字描述符 */
    std::string IP; /**< 节点的IP地址 */
    unsigned short Port = 0; /**< 节点的端口号 */

    /* marks */
    bool Running; /**< 标记节点的运行状态 */
    
};

}
