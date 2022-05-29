#include "Thread.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <utility>
#include "ServerNode.h"
#include "TcpNode.h"

#include "ImGui/IconsFontAwesome.h"


#define PRINT_TO_LOG(type, msg, ...) \
    type##Log.AddLog("[服务端: %s] " msg"\n", Name.c_str(), __VA_ARGS__);

#define PRINT_TO_MSG(type, msg, ...) \
    type##Log.AddLog(msg"\n", __VA_ARGS__);


namespace Network {

void ServerThread::Main()
{
    tcpNode = new TcpNode();
    tcpNode->CreateSocket();
    if (tcpNode->Bind(atoi(Name.c_str())))
    {
        tcpNode->Running = true;
        PRINT_TO_LOG(Status, "%s", "绑定端口成功...");
        StatusInfo = "正常运行-监听中";

    }
    else
    {
        PRINT_TO_LOG(Status, "%s", "绑定端口失败！");
        StatusInfo = "停止运行-端口占用";
        //TODO: 可以增加关闭该服务器的弹出框
        delete tcpNode;
        /* tcpNode = nullptr; */
        return;
    }

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int epfd = epoll_create(256);

    epoll_event ev;
    ev.data.fd = tcpNode->Socket;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, tcpNode->Socket, &ev);

    epoll_event events[20];

    tcpNode->SetBlock(false);
    while (tcpNode->Running)
    {
        int cnt = epoll_wait(epfd, events, 20, 500);
        if (cnt <= 0)
            continue;
        for (int i = 0; i < cnt; i++) {
            if (events[i].data.fd == tcpNode->Socket)
            {
                while (true)
                {
                    Network::TcpNode client = tcpNode->Accept();
                    if (client.Socket <= 0) break;
                    PRINT_TO_LOG(Status, "客户端IP：%s\tPort：%d -> 建立连接...", client.IP.c_str(), client.Port);
                    m_ConnectedClients.insert(std::make_pair(client.Socket, client));

                    ev.data.fd = client.Socket;
                    ev.events = EPOLLIN | EPOLLET;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client.Socket, &ev);
                }
            }
            else
            {
                Network::TcpNode client;
                client = m_ConnectedClients.at(events[i].data.fd);
                /* struct sockaddr_in caddr; */
                /* socklen_t caddr_len = sizeof(caddr); */
                /* getsockname(client.Socket, (struct sockaddr*)&caddr, &caddr_len); */
                if (client.Recv(buf, sizeof(buf)) <= 0)
                {
                    PRINT_TO_LOG(Status, "客户端IP：%s\tPort：%d -> 断开连接!", client.IP.c_str(), client.Port);
                    m_ConnectedClients.erase(events[i].data.fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, client.Socket, &ev);
                }
                else
                {
                    /* PRINT_TO_LOG(Message, "<%d>" ICON_FK_COMMENTING " %s", client.Port, buf); */
                    PRINT_TO_MSG(Message, "<%d>" ICON_FK_COMMENTING " %s", client.Port, buf);
                    client.Send("ok", 3);
                }
            }
        }
    }

    StatusLog.AddLog("等待[端口:%s]的服务器线程结束...\n", Name.c_str());
    tcpNode->Close();
    delete tcpNode;
    tcpNode = nullptr;
}

void ServerThread::UIRenderer(bool *open)
{
    // UI
    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {

        if (!StatusInfo.compare("正常运行-监听中"))
            ImGui::PushStyleColor(0, ImVec4(0.4f, 0.8f, 0.2f, 1.f));
        else
            ImGui::PushStyleColor(0, ImVec4(0.8f, 0.4f, 0.2f, 1.f));
        ImGui::BulletText("服务器[Port %d]状态：%s", tcpNode->Port, StatusInfo.c_str());
        ImGui::PopStyleColor();
        ImGui::EndTabItem();
    }

    MessageLog.Draw(Name.c_str(), open);
    StatusLog.Draw(Name.c_str(), open);
}

}
