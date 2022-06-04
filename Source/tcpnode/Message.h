/**
 * @file Message.h
 * @brief 消息类，保存发送的消息以及附带校验多项式标号
 * @details 提供编码解码函数，以及字符串与二进制相互转换的函数
 */
#pragma once

#include "CheckCode.h"
#include <bitset>


/**
  * @brief 将二进制数据去除高位零，并转为字符串返回
  */
#define BIT_TO_STRING(msg) MAX_BITS - Message::GetLength(msg) < MAX_BITS \
    ? msg.Data.to_string().substr(MAX_BITS - Message::GetLength(msg)).c_str()\
    : "0"

namespace Network
{

/**
  * @brief 服务端与客户端互传数据的结构
  */
class Message
{
public:
    Message() {}

    /**
      * @brief  初始化消息
      * @param[msg] 存入的消息字符串
      * @param[code] 校验所使用的多项式
      */
    Message(const char* msg, CRCCode code);
    ~Message() = default;

    /**
      * @brief  重载除法
      * @param[binary] 除数
      * @return true 模二除法结束
      * @return flase 模二除法还能继续
      */
    bool operator/(std::bitset<MAX_BITS> binary);

    /**
      * @brief  重载左移
      * @param[bits] 左移的位数
      * @return Message& 链式返回
      */
    Message& operator<<(int bits);

    /**
      * @brief  重载右移
      * @param[bits] 右移的位数
      * @return Message& 链式返回
      */
    Message& operator>>(int bits);

    /**
      * @brief  重载异或
      * @param[binary] 与之异或的整形变量
      * @return Message& 链式返回
      */
    Message& operator^(int binary);

    /**
      * @brief  重载异或
      * @param[binary] 与之异或的Message对象
      * @return Message& 链式返回
      */
    Message& operator^(Message& binary);

    /* static function */
    /**
      * @brief  获取保存数据的二进制长度
      * @details 因为二进制数据可能会超过整形的最大保存范围，所以不能简单的通过
      * 将bitset对象转成ulonglong，在log2计算长度，这里用最基本的遍历发计算。
      * @param[binary] 待计算的二进制数
      * @return 二进制数的长度
      * @todo 可以使用更高效的查找法
      */
    static int GetLength(const std::bitset<MAX_BITS>& binary);
    /**
      * @brief  调用上一个函数
      * @param[binary] 待计算的二进制数
      * @return 二进制数的长度
      */
    static int GetLength(const Message& binary);

    /**
      * @brief  将字符串转为二进制
      * @param[msg] 待转换的字符串
      * @param[data] 转换结果的保存变量，最大长度不超过1024位
      */
    static void StringToBits(const char* msg, std::bitset<MAX_BITS>& data);

    /**
      * @brief  将二进制转为字符串
      * @param[data] 待转换的二进制数
      */
    static std::string BitsToString(const std::bitset<MAX_BITS>& data);

    /**
      * @brief  为传入的消息生成CRC校验码
      * @param[msg] 待计算的消息
      */
    static void Encode(Message& msg);

    /**
      * @brief  校验传入的消息
      * @param[msg] 待校验的消息
      * @return true 校验通过
      * @return flase 校验错误
      */
    static bool Decode(Message& msg);

public:
    std::bitset<MAX_BITS> Data; /**< 消息中保存的二进制数据 */
    CRCCode Code; /**< 校验消息需使用的CRC多项式 */
};

}
