#include "Application.h"

#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "TcpNode.h"
#include "imgui.h"
#include "ClientThread.h"

static bool createClient = false;

std::unordered_map<std::string, Network::Thread*> ClientThreads;

//TODO: 创建一个新的线程对象到服务端线程列表中
void CreateClientUI(std::unordered_map<std::string, Network::Thread*>* tabs);
void ClientUI();

int main()
{

    Application app(600, 950);

    app.Init();

    app.Run(ClientUI);

    for (auto& Client : ClientThreads) {
        if (Client.second)
        {
            if (Client.second->tcpNode)
                Client.second->tcpNode->Running = false;
            delete Client.second;
        }
    }

    return 0;
}

void ClientUI()
{
    // UI
    /* ImGui::ShowDemoWindow(); */

    ImGui::Begin("客户端");
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;

    static bool show_leading_button = false;
    static bool show_trailing_button = true;
    // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                                            ImGuiTabBarFlags_Reorderable | 
                                            ImGuiTabBarFlags_FittingPolicyResizeDown |
                                            ImGuiTabBarFlags_TabListPopupButton;
    if (ImGui::BeginTabBar("ClientTagBar", tab_bar_flags))
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
                createClient = true;
            }
        if (createClient)
            CreateClientUI(&ClientThreads);
            /* CreateClientUI(&active_tabs, &next_tab_id); */
        if (!active_tabs.Size && !ClientThreads.size())
        {
            ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建客户端").x) / 2.f);
            ImGui::SetCursorPosY( (ImGui::GetWindowHeight() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建客户端").y) / 2.f);
            ImGui::Text("点击 " ICON_FK_PLUS_SQUARE " 创建客户端");
        }

        // Submit our regular tabs
        for (auto& Client : ClientThreads) {
            bool open = true;
            if (Client.second)
            {
                Client.second->UIRenderer(&open);
                if (!open)
                {
                    //TODO: Remove Client thread object
                    if (Client.second->tcpNode)
                        Client.second->tcpNode->Running = false;
                    ClientThreads.erase(Client.second->Name);
                    delete Client.second;
                    break;
                }
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

void CreateClientUI(std::unordered_map<std::string, Network::Thread*>* tabs)
{
    //TODO: 添加创建服务端所需的组件

    ImGui::OpenPopup("创建客户端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("创建客户端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入客户端名称\n\n");
        ImGui::Separator();

        char name[10] = {0};
        static std::string tag;
        if(ImGui::InputTextWithHint("名称", tag.c_str(), name, sizeof(name)))
        {
            tag = name;
        }

        if (ImGui::Button("OK", ImVec2(80, 0))) 
        { 
            std::cout << "名称" << tag << std::endl;
            tabs->insert(std::make_pair(tag, new Network::ClientThread(tag))); // Add new tab
            createClient = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80, 0)))
        { 
            createClient = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::EndPopup();
    }
}
