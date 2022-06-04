/**
 * @file CheckCode.h
 * @brief CRC多项式
 * @details 方便根据枚举值获取CRC多项式信息
 */
#pragma once

#define MAX_BITS 1024

#include <string>
#include <bitset>

namespace Network
{

/**
* @brief CRC校验多项式的枚举值
*/
enum class CRCCode
{
    CRC_4=0, /**< CRC_4 */
    CRC_8, /**< CRC_8 */
    CRC_12, /**< CRC_12 */
    CRC_16, /**< CRC_16 */
    CRC_32, /**< CRC_32 */
    CRC_CCITT, /**< CRC_CCITT */
};

/**
* @brief 根据CRC多项式的枚举值获得对应名称
* @param[code] CRC多项式枚举值
* @return 对应名称
*/
static std::string GetCRCCodeName(CRCCode code)
{
    switch (code)
    {
    case CRCCode::CRC_4:
        return "CRC_4";
    case CRCCode::CRC_8:
        return "CRC_8";
    case CRCCode::CRC_12:
        return "CRC_12";
    case CRCCode::CRC_16:
        return "CRC_16";
    case CRCCode::CRC_32:
        return "CRC_32";
    case CRCCode::CRC_CCITT:
        return "CRC_CCITT";
    }
    return "None";
}

/**
* @brief 获取对应CRC多项式枚举值的二进制表示
* @param[code] CRC多项式枚举值
* @return 对应二进制表示
*/
static std::bitset<MAX_BITS> GetCheckCode(CRCCode code)
{
    std::bitset<MAX_BITS> crcCode = 0b11;
    switch (code)
    {
    case CRCCode::CRC_4:
        crcCode = 0b11001;
        break;
    case CRCCode::CRC_8:
        crcCode = 0b100110001;
        break;
    case CRCCode::CRC_12:
        crcCode = 0b1100000001101;
        break;
    case CRCCode::CRC_16:
        crcCode = 0b11000000000000101;
        break;
    case CRCCode::CRC_32:
        crcCode = 0b100000100110000010001110110110111;
        break;
    case CRCCode::CRC_CCITT:
        crcCode = 0b10001000000100001;
        break;
    }
    return crcCode;
}

}
