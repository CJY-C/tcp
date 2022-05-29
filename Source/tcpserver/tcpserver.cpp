#include "Application.h"

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "TcpNode.h"
#include "imgui.h"
#include "Thread.h"

static bool createServer = false;

std::unordered_map<std::string, Network::Thread*> serverThreads;

//TODO: 创建一个新的线程对象到服务端线程列表中
void CreateServerUI(std::unordered_map<std::string, Network::Thread*>* tabs);
void ServerUI();

int main()
{

    Application app(600, 950);

    app.Init();

    app.Run(ServerUI);

    for (auto& server : serverThreads) {
        if (server.second)
        {
            if (server.second->tcpNode)
                server.second->tcpNode->Running = false;
            delete server.second;
        }
    }

    return 0;
}

void ServerUI()
{
    // UI
    /* ImGui::ShowDemoWindow(); */

    ImGui::Begin("服务端");
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;

    static bool show_leading_button = false;
    static bool show_trailing_button = true;
    // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                                            ImGuiTabBarFlags_Reorderable | 
                                            ImGuiTabBarFlags_FittingPolicyResizeDown |
                                            ImGuiTabBarFlags_TabListPopupButton;
    if (ImGui::BeginTabBar("ServerTagBar", tab_bar_flags))
    {
        // Demo a Leading TabItemButton(): click the "?" button to open a menu
        if (show_leading_button)
            if (ImGui::TabItemButton("?", ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_NoTooltip))
                ImGui::OpenPopup("MyHelpMenu");
        if (ImGui::BeginPopup("MyHelpMenu"))
        {
            ImGui::Selectable("Hello!");
            ImGui::EndPopup();
        }

        // Demo Trailing Tabs: click the "+" button to add a new tab (in your app you may want to use a font icon instead of the "+")
        // Note that we submit it before the regular tabs, but because of the ImGuiTabItemFlags_Trailing flag it will always appear at the end.
        if (show_trailing_button)
            if (ImGui::TabItemButton(ICON_FK_PLUS_SQUARE, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
            {
                createServer = true;
            }
        if (createServer)
            CreateServerUI(&serverThreads);
            /* CreateServerUI(&active_tabs, &next_tab_id); */
        if (!active_tabs.Size && !serverThreads.size())
        {
            ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建服务端").x) / 2.f);
            ImGui::SetCursorPosY( (ImGui::GetWindowHeight() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建服务端").y) / 2.f);
            ImGui::Text("点击 " ICON_FK_PLUS_SQUARE " 创建服务端");
        }

        // Submit our regular tabs
        for (auto& server : serverThreads) {
            bool open = true;
            if (server.second)
            {
                server.second->UIRenderer(&open);
                if (!open)
                {
                    //TODO: Remove server thread object
                    if (server.second->tcpNode)
                        server.second->tcpNode->Running = false;
                    serverThreads.erase(server.second->Name);
                    delete server.second;
                    break;
                }
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void CreateServerUI(std::unordered_map<std::string, Network::Thread*>* tabs)
{
    //TODO: 添加创建服务端所需的组件

    ImGui::OpenPopup("创建服务端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("创建服务端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入服务器端口号\n\n");
        ImGui::Separator();

        char port[10] = {0};
        static std::string tag;
        if(ImGui::InputTextWithHint("Port", tag.c_str(), port, sizeof(port)))
        {
            tag = port;
        }

        //static int unused_i = 0;
        //ImGui::Combo("Combo", &unused_i, "Delete\0Delete harder\0");

        /* static bool dont_ask_me_next_time = false; */
        /* ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); */
        /* ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time); */
        /* ImGui::PopStyleVar(); */

        if (ImGui::Button("OK", ImVec2(80, 0))) 
        { 
            std::cout << "port:" << atoi(tag.c_str()) << std::endl;
            tabs->insert(std::make_pair(tag, new Network::ServerThread(tag))); // Add new tab
            createServer = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80, 0)))
        { 
            createServer = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::EndPopup();
    }
}
