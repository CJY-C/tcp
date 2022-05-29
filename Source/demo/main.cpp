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

#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "implot.h"


#include "Server.h"
#include "Client.h"

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static Network::Server server("127.0.0.1", 5005);
static Network::Client client;

void ShowDemo_LinePlots() {
    static float xs1[1001], ys1[1001];
    for (int i = 0; i < 1001; ++i) {
        xs1[i] = i * 0.001f;
        ys1[i] = 0.5f + 0.5f * sinf(50 * (xs1[i] + (float)ImGui::GetTime() / 10));
    }
    static double xs2[11], ys2[11];
    for (int i = 0; i < 11; ++i) {
        xs2[i] = i * 0.1f;
        ys2[i] = xs2[i] * xs2[i];
    }
    ImGui::BulletText("Anti-aliasing can be enabled from the plot's context menu (see Help).");
    if (ImPlot::BeginPlot("Line Plot")) {
        ImPlot::SetupAxes("x","f(x)");
        ImPlot::PlotLine("sin(x)", xs1, ys1, 1001);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::PlotLine("x^2", xs2, ys2, 11);
        ImPlot::EndPlot();
    }
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1600, 900, "CRC Checker Demo", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
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
    
    // font
    io.FontDefault = io.Fonts->AddFontFromFileTTF("./Resources/Fonts/Noto_Sans_SC/NotoSansSC-Thin.otf", 30.f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    // Our state
    bool show_log_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // homework start here!!
    bool disable = false;

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
        ShowDemo_LinePlots();
        /* memset(buffer, 0, sizeof(buffer)); */
        if (client.Sending)
        {
            ImGui::BeginDisabled();
            disable = true;
        }
        ImGui::Begin("TCP Demo");
        if (ImGui::Button("Stop Server"))
        {
            server.Shutdown();
        }
        if (ImGui::Button("Client connect to Server"))
        {
            client.Connect(&server);
        }
        if (ImGui::Button("Client send msg to Server"))
        {
            client.Send();
        }
        if (ImGui::Button("Stop Client"))
        {
            client.Shutdown();
        }

        if (ImGui::Button(u8"关闭"))
        {
            break;
        }
        ImGui::End();
        if (disable)
        {
            ImGui::EndDisabled();
            disable = false;
        }

        /* ImGui::Begin("CRC Checker Demo"); */


        /* ImGui::PushItemWidth(200); */
        /* ImGui::Combo("##Combo", &item_current, items, IM_ARRAYSIZE(items)); */
        /* ImGui::PopItemWidth(); */


        /* ImGui::SameLine(); */ 
        /* ImGui::Text("Data:"); */
        /* ImGui::SameLine(); */ 
        /* ImGui::PushItemWidth(400); */
        /* if (ImGui::InputTextWithHint("##Tag", tag.c_str(), buffer, sizeof(buffer))) */
        /*     tag = std::string(buffer); */
        /* ImGui::PopItemWidth(); */

        /* ImGui::SameLine(); */
        /* ImGui::Text("Noise:"); */

        /* ImGui::SameLine(); */
        /* ImGui::PushItemWidth(200); */
        /* if (ImGui::SliderFloat("##Noise", &noise, 0.0f, 1.0f)) */
        /*     bus.SetNoise(noise); */
        /* ImGui::PopItemWidth(); */

        /* ImGui::SameLine(); */ 
        /* if (ImGui::Button("Send")) */
        /*     if (!tag.empty()) */
        /*     { */
        /*         std::bitset<MAX_BIT> binary(tag.c_str()); */
        /*         long long data = binary.to_ullong(); */
        /*         client->SendMessage(data, 2, (Network::CRCCode)item_current); */
        /*         tag.clear(); */
        /*     } */

        /* ImGui::SameLine(); */ 
        /* if (ImGui::Button("Clear All")) */
        /*     bus.ClearAllLogs(); */

        /* std::bitset<MAX_BIT> code_current = Network::GetCheckCode((Network::CRCCode)item_current); */
        /* ImGui::Text("Check Code: %s", code_current.to_string().substr(MAX_BIT - Network::GetLength(code_current)).c_str()); */               

        /* ImGui::End(); */

        /* bus.OnUpdate(); */
        /* if (show_log_window) */
        /* { */
        /*     bus.OnImGuiRenderer(); */
        /* } */
        ImGui::ShowDemoWindow();

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

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
