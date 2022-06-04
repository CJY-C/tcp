/**
 * * @file Application.h
 * * @brief 简单封装的UI界面类
 */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

typedef struct GLFWwindow GLFWwindow;

/**
  * @brief  提供一个UI框架
  * @details 创建窗口、添加组件
  */
class Application
{
public:
    Application() = default;
    /**
      * @brief  创建指定宽和高的应用窗口
      * @param[width]  宽默认1200
      * @param[height] 高默认750
      */
    Application(int width, int height) : m_Width(width), m_Height(height){}
    ~Application() { Stop(); };

    /**
      * @brief  各种初始化（从UI库例程中复制粘贴）
      */
    void Init();

    /**
      * @brief  主循环（从UI库例程中复制粘贴）
      * @param[func] 传入一个函数指针，将自己的程序嵌入到所用的框架中
      */
    void Run(void(*func)());

    /**
      * @brief  结束应用程序释放资源
      */
    void Stop();

private:
    GLFWwindow* window;
    int m_Width=1200, m_Height=750;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static Application* s_Instance;
};
