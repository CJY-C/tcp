#include "Application.h"
#include "ServerThread.h"

#include "ImGui/ImLog.h"
#include "ImGui/IconsFontAwesome.h"
#include "ImGui/MyComponents.h"
#include "imgui.h"

#include <vector>
#include <unordered_map>


static bool createServer = false;
std::unordered_map<std::string, Network::TCPThread*> serverThreads;

void CreateServerUI(std::unordered_map<std::string, Network::TCPThread*>* tabs);
void ServerUI();

int main()
{

    Application app(600, 950);

    app.Init();

    app.Run(ServerUI);

    for (auto& server : serverThreads) {
        if (server.second)
        {
            if (server.second->Node)
                server.second->Node->Running = false;
            delete server.second;
        }
    }

    return 0;
}

void ServerUI()
{
    ImGui::Begin("服务端");
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;

    static bool show_trailing_button = true;
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                                            ImGuiTabBarFlags_Reorderable | 
                                            ImGuiTabBarFlags_FittingPolicyResizeDown |
                                            ImGuiTabBarFlags_TabListPopupButton;
    if (ImGui::BeginTabBar("ServerTagBar", tab_bar_flags))
    {
        if (show_trailing_button)
            if (ImGui::TabItemButton(ICON_FK_PLUS_SQUARE, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                createServer = true;

        if (createServer)
            CreateServerUI(&serverThreads);

        if (!active_tabs.Size && !serverThreads.size())
        {
            ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建服务端").x) / 2.f);
            ImGui::SetCursorPosY( (ImGui::GetWindowHeight() - ImGui::CalcTextSize("点击 " ICON_FK_PLUS_SQUARE " 创建服务端").y) / 2.f);
            ImGui::Text("点击 " ICON_FK_PLUS_SQUARE " 创建服务端");
        }

        for (auto& server : serverThreads) 
        {
            bool open = true;
            if (server.second)
            {
                server.second->UIRenderer(&open);
                if (!open)
                {
                    if (server.second->Node)
                        server.second->Node->Running = false;
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

void CreateServerUI(std::unordered_map<std::string, Network::TCPThread*>* tabs)
{
    ImGui::OpenPopup("创建服务端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("创建服务端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入服务器端口号");
        ImGui::Separator();

        static std::string tag;
        InputText("端口号", 250, tag, 7);

        CheckOrCancel(
        [&](){
            // check
            tabs->insert(std::make_pair(tag, new Network::ServerThread(tag)));
            createServer = false;
            ImGui::CloseCurrentPopup(); 
        }, 
        [&](){
            // cancel
            createServer = false;
            ImGui::CloseCurrentPopup(); 
        });
        ImGui::EndPopup();
    }
}
