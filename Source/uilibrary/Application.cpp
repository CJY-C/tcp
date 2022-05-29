#include "Application.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <bitset>
#include <cmath>

#include "ImGui/ImLog.h"
#include "ImGui/ImScene.h"
#include "ImGui/IconsFontAwesome.h"


Application* Application::s_Instance = nullptr;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void Application::Init()
{
    std::cout << "Set up!" << std::endl;
    s_Instance = this;

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    window = glfwCreateWindow(m_Width, m_Height, "CRC Checker Demo", NULL, NULL);
    if (window == NULL)
        return;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    SetDarkThemeColors();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef RESOURCE_ROOT

    std::string fontPath = RESOURCE_ROOT;
    fontPath.append("/Fonts/Noto_Sans_SC/NotoSansSC-Thin.otf");
    // font
    io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 30.f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    // Merge icons into default tool font
    /* ImGuiIO& io = ImGui::GetIO(); */
    /* io.Fonts->AddFontDefault(); */

    ImFontConfig config;
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 30.0f; // Use if you want to make the icon monospaced
    std::string iconPath = RESOURCE_ROOT;
    iconPath.append("/Fonts/fontawesome-webfont.ttf");
    static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
    /* io.Fonts->AddFontFromFileTTF(iconPath.c_str(), 13.0f, &config); */
    io.Fonts->AddFontFromFileTTF(iconPath.c_str(), 30.0f, &config, icon_ranges);

#else
    std::cout << "!!" << std::endl;
#endif
    // Our state
    bool show_log_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

}

void Application::Run(void(*func)())
{
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        func();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    	
        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

}

void Application::Stop()
{
    std::cout << "Clean up !" << std::endl;
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
