#include "Application.h"
#include "CheckCode.h"
#include "ClientThread.h"

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "ImGui/MyComponents.h"
#include "imgui.h"

#include <string>
#include <vector>
#include <unordered_map>


static bool createClient = false;
std::unordered_map<std::string, Network::TCPThread*> ClientThreads;

void CreateClientUI(std::unordered_map<std::string, Network::TCPThread*>* tabs);
void ClientUI();

int main()
{

    Application app(600, 950);

    app.Init();

    app.Run(ClientUI);

    for (auto& Client : ClientThreads) {
        if (Client.second)
        {
            if (Client.second->Node)
                Client.second->Node->Running = false;
            delete Client.second;
        }
    }

    return 0;
}

void ClientUI()
{
    ImGui::Begin("客户端");
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;

    static bool show_trailing_button = true;
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                                            ImGuiTabBarFlags_Reorderable | 
                                            ImGuiTabBarFlags_FittingPolicyResizeDown |
                                            ImGuiTabBarFlags_TabListPopupButton;
    if (ImGui::BeginTabBar("ClientTagBar", tab_bar_flags))
    {
        if (show_trailing_button)
            if (ImGui::TabItemButton(ICON_FK_PLUS_SQUARE, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                createClient = true;
        if (createClient)
            CreateClientUI(&ClientThreads);

        if (!active_tabs.Size && !ClientThreads.size())
        {
            ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建客户端").x) / 2.f);
            ImGui::SetCursorPosY( (ImGui::GetWindowHeight() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建客户端").y) / 2.f);
            ImGui::Text("点击 " ICON_FK_PLUS_SQUARE " 创建客户端");
        }

        for (auto& Client : ClientThreads) {
            bool open = true;
            if (Client.second)
            {
                Client.second->UIRenderer(&open);
                if (!open)
                {
                    if (Client.second->Node)
                        Client.second->Node->Running = false;
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

void CreateClientUI(std::unordered_map<std::string, Network::TCPThread*>* tabs)
{
    ImGui::OpenPopup("创建客户端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("创建客户端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入客户端名称\n");
        ImGui::Separator();

        static std::string tag;
        InputText("名称", 250, tag, 32);

        CheckOrCancel(
                // check
                [&](){
                    tabs->insert(std::make_pair(tag, new Network::ClientThread(tag)));
                    createClient = false;
                }, 
                // cancel
                [&](){
                    createClient = false;
                });
        ImGui::EndPopup();
    }
}
