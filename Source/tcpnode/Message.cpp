#include "Message.h"

#include <string>
#include <cstring>
#include <bitset>
#include <stack>



namespace Network
{
    Message::Message(const char* msg, CRCCode code)
        : Code(code) 
    { 
        char validMsg[1024] = " ";
        strcat(validMsg, msg);
        StringToBits(validMsg, Data); 
    }

    bool Message::operator/(std::bitset<MAX_BITS> binary)
    {
        int selfLength = GetLength(*this);
        int dividerLength = GetLength(binary);
        binary <<= selfLength - dividerLength;
        if (selfLength < dividerLength)
            return false;
        Data ^= binary;
        return true;
    }

    Message& Message::operator^(int binary)
    {
        Data ^= binary;
        return *this;
    }
    Message& Message::operator^(Message& binary)
    {
        Data ^= binary.Data;
        return *this;
    }

    Message& Message::operator<<(int bits)
    {
        this->Data <<= bits;
        return *this;
    }
    Message& Message::operator>>(int bits)
    {
        this->Data >>= bits;
        return *this;
    }

    void Message::Encode(Message& msg)
    {
        std::bitset<MAX_BITS> divider = GetCheckCode(msg.Code);
        Message process = msg;

        int dividerLength = GetLength(divider);
        process << dividerLength - 1;

        while (process / divider) {}
        msg << dividerLength - 1;
        msg ^ process;
    }

    bool Message::Decode(Message& msg)
    {
        Message justify = msg;
        std::bitset<MAX_BITS> divider = GetCheckCode(msg.Code);

        while (justify / divider) {}

        if (justify.Data.count())
            return false;
        msg >> (GetLength(divider) - 1);
        return true;
    }

    int Message::GetLength(const std::bitset<MAX_BITS>& binary)
    {
        //TODO: 加速算法
        int one_cnt = binary.count();
        int len = 0;
        for (int i = 0; i < binary.size(); i++) {
            if (0 == one_cnt)
                break;
            if (binary.test(i))
                one_cnt--;
            len++;
        }
        return len;
    }

    int Message::GetLength(const Message& binary)
    {
        return GetLength(binary.Data);
    }

    void Message::StringToBits(const char* msg, std::bitset<MAX_BITS>& data)
    {
        std::string tmp(msg);
        std::string bits = "";
        for (auto word : tmp) 
            bits += std::bitset<8>(word).to_string();
        data = std::bitset<MAX_BITS>(bits);
    }

    std::string Message::BitsToString(const std::bitset<MAX_BITS>& data)
    {
        std::string msg = "";
        auto copy = data;
        auto test = std::bitset<MAX_BITS>(255);
        std::stack<char> words; 

        for (int i = 0; i < GetLength(data)/8 + 1; i++) {
            test &= copy;
            words.push(test.to_ulong());
            copy >>= 8;
            test = std::bitset<MAX_BITS>(255);
        }

        while (!words.empty())
        {
            msg.append(1, words.top());
            words.pop();
        }

        return msg;
    }

}
