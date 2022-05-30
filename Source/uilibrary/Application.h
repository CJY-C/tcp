#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

typedef struct GLFWwindow GLFWwindow;

class Application
{
public:
    Application() = default;
    Application(int width, int height) : m_Width(width), m_Height(height){}
    ~Application() { Stop(); };

    void Init();

    void Run(void(*func)());

    void Stop();

private:
    GLFWwindow* window;
    int m_Width=1200, m_Height=750;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static Application* s_Instance;
    
};
