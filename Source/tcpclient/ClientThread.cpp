#include "ClientThread.h"
#include "CheckCode.h"
#include "ClientNode.h"
#include "Message.h"

#include "imgui.h"
#include "ImGui/IconsFontAwesome.h"
#include "ImGui/MyComponents.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/tcp.h>
#include <thread>
#include <unistd.h>
#include <sys/epoll.h>


//TODO: 格式化修改
#define PRINT_TO_LOG(type, msg, ...) \
    type##Log.AddLog("[客户端: %s] " msg"\n", Name.c_str(), __VA_ARGS__);

#define PRINT_TO_MSG(type, msg, ...) \
    type##Log.AddLog(msg"\n", __VA_ARGS__);


namespace Network {

ClientThread::ClientThread(std::string name)
    :TCPThread(name)
{
    for (int i = 0; i < 6; i++) 
    {
        std::string codeName = Network::GetCRCCodeName((Network::CRCCode)i);
        m_CRCCodeNames[i] = (char*)malloc(sizeof(char) * strlen(codeName.c_str()));
        strcpy(m_CRCCodeNames[i], codeName.c_str());
    }
}

void ClientThread::Main()
{
    /* 创建套接字 */
    Node = new ClientNode();
    Node->CreateSocket();
    Node->Running = true;
    StatusInfo = "正常运行-未连接";

    /* 监控是否进行丢包重发的线程 */
    m_MonitorThread = std::thread(&ClientThread::MonitorRetrans, this);
    m_MonitorThread.detach();

    m_epfd = epoll_create(255);

    Node->SetBlock(false); /* 将客户端套接字设置成非阻塞模式 */

    while (Node->Running) /* 客户端主循环 */
    {
        int cnt = epoll_wait(m_epfd, m_Events, 20, 200); /* 阻塞200ms，监听事件 */
        if (cnt <= 0)
            continue;

        for (int i = 0; i < cnt; i++) 
        {
            m_Event = m_Events[i];

            ReceiveMessage(); /* 接收发来的消息 */
        }
    }

    m_Connected = false;

}

void ClientThread::UIRenderer(bool *open)
{

    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {
        ClientStatusUI();
        ImGui::EndTabItem();
    }

    MessageLog.Draw(Name.c_str(), open);
    StatusLog.Draw(Name.c_str(), open);

    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {
        ClientSendingUI();
    }
}


void ClientThread::ConnectingServerUI()
{
    ImGui::OpenPopup("连接服务端");

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("连接服务端", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("请输入服务器地址以及端口号");
        ImGui::Separator();

        static std::string inputIP="127.0.0.1";
        InputText("IP地址", 200, inputIP, 10);
        ImGui::SameLine();
        static std::string inputPort;
        InputText("端口号", 100, inputPort, 7);

        CheckOrCancel(
        // check
        [&](){
            port = atoi(inputPort.c_str());
            m_ConfigServer = false;
            if (Node->Connect(inputIP, port))
            {
                StatusInfo = "正常运行-已连接";
                m_Connected = true;

                m_Event.data.fd = Node->Socket;
                m_Event.events = EPOLLIN | EPOLLET;
                epoll_ctl(m_epfd, EPOLL_CTL_ADD, Node->Socket, &m_Event);

                Message name(Name.c_str(), (CRCCode)m_CurrentCode);
                Node->Send(&name, sizeof(name));

                PRINT_TO_LOG(Status, "服务端[%d] 连接成功", port);
            }
            else
            {
                PRINT_TO_LOG(Status, "无法连接到端口号[%d]！", port);
                m_Connected = false;
                StatusInfo = "正常运行-未连接";
            }
        }, 
        // cancel
        [&](){
            m_ConfigServer = false;
        });

        ImGui::EndPopup();
    }
}

void ClientThread::ClientStatusUI()
{
    //TODO: 优雅一点
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
            //TODO: 断开连接，错误处理
            epoll_ctl(m_epfd, EPOLL_CTL_DEL, Node->Socket, &m_Event);
            Node->Disconnect();
            m_Connected = false;
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
            m_ConfigServer = true;
        }
    }
    if (m_ConfigServer)
        ConnectingServerUI();
}

void ClientThread::ClientSendingUI()
{

    ImGui::PushItemWidth(150);
    ImGui::Combo("##Combo", &m_CurrentCode, m_CRCCodeNames, IM_ARRAYSIZE(m_CRCCodeNames));
    ImGui::PopItemWidth();

    ImGui::SameLine();
    InputText("", 350, m_msg, 1024);

    ImGui::SameLine();
    if (!m_Connected)
        ImGui::BeginDisabled();
    if (ImGui::Button(ICON_FK_PAPER_PLANE))
    {
        Message msg(m_msg.c_str(), (CRCCode)m_CurrentCode);
        Message::Encode(msg);
        Node->Send(&msg, sizeof(msg));
        PRINT_TO_LOG(Status, "发送:\n%s", BIT_TO_STRING(msg));
        /* 一次收发结束 */
        m_msg.clear();
        /* m_SendEvent = true; */

    }
    if (!m_Connected)
        ImGui::EndDisabled();

    ImGui::EndTabItem();
}

void ClientThread::MonitorRetrans()
{
    struct tcp_info info;
    int length=sizeof(struct tcp_info);
    int currentRetrans = 0; /* 记录当前重发次数 */
    int lastRetrans = 0; /* 上次一重发次数 */
    while (Node->Running) 
    {
        while (m_Connected)
        if(getsockopt(Node->Socket, IPPROTO_TCP, 
                      TCP_INFO, (void*)&info, (socklen_t*)&length)==0)
        {
            currentRetrans = info.tcpi_retransmits;

            if (lastRetrans != currentRetrans)
            {
                if (currentRetrans != 0)
                    PRINT_TO_LOG(Status, " 丢包重发[次数 %d]!", currentRetrans);
                lastRetrans = currentRetrans;
            }
        }
    }
}

void ClientThread::ReceiveMessage()
{
    char recvBuf[1024] = {0};
    if (Node->Recv(recvBuf, sizeof(recvBuf)))
    {
        if (!strcmp(recvBuf, "close"))
        {
            PRINT_TO_LOG(Status, "服务端[%d] %s", 
                    port, "断开连接！");
            StatusInfo = "正常运行-未连接";
            m_Connected = false;
        }
        else
        PRINT_TO_MSG(Message, "%s", recvBuf);
    }
}

}
