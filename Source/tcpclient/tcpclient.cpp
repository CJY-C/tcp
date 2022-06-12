#include "EntryPoint.h"
#include "ClientThread.h"

std::unordered_map<std::string, Network::TCPThread*> TCPThreads;
bool newThread = false;
std::string name = "客户端";

void newTCPThread(std::unordered_map<std::string, Network::TCPThread*>* tabs)
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
            newThread = false;
        }, 
        // cancel
        [&](){
            newThread = false;
        });
        ImGui::EndPopup();
    }
}
