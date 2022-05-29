#include "ClientThread.h"
#include "ClientNode.h"
#include "imgui.h"
#include "ImGui/IconsFontAwesome.h"
#include <cstdlib>
#include <cstring>

#define PRINT_TO_LOG(type, msg, ...) \
    type##Log.AddLog("[客户端: %s] " msg"\n", Name.c_str(), __VA_ARGS__);

#define PRINT_TO_MSG(type, msg, ...) \
    type##Log.AddLog(msg"\n", __VA_ARGS__);


namespace Network {

void ClientThread::Main()
{
    tcpNode = new ClientNode();
    tcpNode->CreateSocket();
    tcpNode->Running = true;
    StatusInfo = "正常运行-未连接";

    while (tcpNode->Running)
    {
        /* if (tcpNode->GetActiveEvent().to_ulong()) */
        /*     std::cout << "events: " << tcpNode->GetActiveEvent().to_string() << std::endl; */
        if (tcpNode->Sending())
        {
            //TODO: 处理发送数据事件
            std::cout << "发送数据到服务端" << std::endl;
            char sendBuf[1024] = {0};
            strcpy(sendBuf, m_msg.c_str());
            tcpNode->Send(sendBuf, strlen(sendBuf) + 1);
            PRINT_TO_LOG(Status, "发送: %s", sendBuf);

            char recvBuf[1024] = {0};
            tcpNode->Recv(recvBuf, sizeof(recvBuf));
            PRINT_TO_MSG(Message, "<%d>" ICON_FK_COMMENTING " %s", port, recvBuf);

            m_msg.clear();
        }
    }

    tcpNode->Close();
    delete tcpNode;
}

void ClientThread::UIRenderer(bool *open)
{
    // UI
    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {

        if (!StatusInfo.compare("正常运行-已连接"))
        {
            ImGui::PushStyleColor(0, ImVec4(0.4f, 0.8f, 0.2f, 1.f));
            ImGui::BulletText("客户端[%s]状态：%s", Name.c_str(), StatusInfo.c_str());
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FK_CHAIN_BROKEN).x* 2);
            ImGui::PopStyleColor();
            if (ImGui::Button(ICON_FK_CHAIN_BROKEN, ImVec2(
                    ImGui::CalcTextSize(ICON_FK_CHAIN_BROKEN).x* 1.2,
                    ImGui::CalcTextSize(ICON_FK_CHAIN_BROKEN).y* 1
                            )))
            {
                //TODO: 断开连接
                tcpNode->TriggerDisconnect();
                StatusInfo = "正常运行-未连接";
            }

        }
        else
        {
            ImGui::PushStyleColor(0, ImVec4(0.8f, 0.4f, 0.2f, 1.f));
            ImGui::BulletText("客户端[%s]状态：%s", Name.c_str(), StatusInfo.c_str());
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FK_LINK).x* 2);
            ImGui::PopStyleColor();
            if (ImGui::Button(ICON_FK_LINK, ImVec2(
                    ImGui::CalcTextSize(ICON_FK_LINK).x* 1.2,
                    ImGui::CalcTextSize(ICON_FK_LINK).y* 1
                            )))
            {
                m_configServer = true;
            }
        }

        if (m_configServer)
            ConnectingServerUI();
        /* ImGui::BulletText("客户端[Port %d]状态：%s", tcpNode->Port, StatusInfo.c_str()); */
        ImGui::EndTabItem();
    }

    MessageLog.Draw(Name.c_str(), open);
    StatusLog.Draw(Name.c_str(), open);

    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {
        char msg[10] = {0};
        ImGui::SetCursorPosX(80);
        if(ImGui::InputTextWithHint("##msg", m_msg.c_str(), msg, sizeof(msg)))
        {
            m_msg = msg;
        }
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(ICON_FK_PAPER_PLANE).x * 2);
        if (ImGui::Button(ICON_FK_PAPER_PLANE))
        {
            //TODO: Send message
            tcpNode->TriggerSend();
            std::cout << "发送： " << m_msg << std::endl;
        }
        ImGui::EndTabItem();
    }
}

void ClientThread::ConnectingServerUI()
{
    //TODO: 添加创建服务端所需的组件

    ImGui::OpenPopup("连接服务端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("连接服务端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入服务器端口号\n\n");
        ImGui::Separator();

        char input[10] = {0};
        static std::string tag;
        if(ImGui::InputTextWithHint("Port", tag.c_str(), input, sizeof(input)))
        {
            tag = input;
        }

        if (ImGui::Button("OK", ImVec2(80, 0))) 
        { 
            port = atoi(tag.c_str());
            std::cout << "Port" << tag << std::endl;
            m_configServer = false;
            if (tcpNode->Connect("127.0.0.1", port))
            {
                StatusInfo = "正常运行-已连接";
                PRINT_TO_LOG(Status, "服务端[%d] 连接成功", port);
            }
            else
            {
                PRINT_TO_LOG(Status, "端口: %d 连接失败，请输入正确服务端端口", port);
                StatusInfo = "正常运行-未连接";
            }
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(80, 0)))
        { 
            m_configServer = false;
            ImGui::CloseCurrentPopup(); 
        }
        ImGui::EndPopup();
    }
}

}
