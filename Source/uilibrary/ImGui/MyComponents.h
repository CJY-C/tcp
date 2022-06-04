/**
 * @file MyComponents.h
 * @brief 定义的一些共用的UI组件
 */
#pragma once

#include <bits/stdint-uintn.h>
#include <functional>
#include <string>

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "imgui.h"


/**
  * @brief   弹出框确认取消组件
  * @param[check_func]  点击确认将运行的函数
  * @param[cancel_func] 点击取消将运行的函数
  */
void CheckOrCancel(std::function<void()> check_func, std::function<void()> cancel_func);

/**
  * @brief   输入框组件
  * @param[title] 输入框标签
  * @param[width] 输入框宽度
  * @param[tag]   保存输入的信息
  * @param[maxLength]   单次输入的最大字符数
  */
void InputText(const char* title, unsigned short width, std::string& tag, const uint32_t maxLength);
