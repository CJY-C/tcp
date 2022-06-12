#include "EntryPoint.h"
#include "ServerThread.h"

std::unordered_map<std::string, Network::TCPThread*> TCPThreads;
bool newThread = false;
std::string name = "服务端";

void newTCPThread(std::unordered_map<std::string, Network::TCPThread*>* tabs)
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
            newThread = false;
        }, 
        [&](){
            // cancel
            newThread = false;
        });
        ImGui::EndPopup();
    }
}
