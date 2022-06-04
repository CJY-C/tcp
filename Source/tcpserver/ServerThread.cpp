#include "ServerThread.h"

#include "Message.h"
#include "ServerNode.h"

#include "ImGui/IconsFontAwesome.h"
#include "imgui.h"

#include <cerrno>
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



#define PRINT_TO_LOG(type, msg, ...) \
    type##Log.AddLog("[服务端: %s] " msg"\n", Name.c_str(), __VA_ARGS__);

#define PRINT_TO_MSG(type, msg, ...) \
    type##Log.AddLog(msg"\n", __VA_ARGS__);


namespace Network {

void ServerThread::Main()
{
    /* 创建套接字 */
    Node = new ServerNode();
    Node->CreateSocket();

    /* 绑定端口 */
    if (Node->Bind(atoi(Name.c_str())))
    {
        Node->Running = true;
        PRINT_TO_LOG(Status, "%s", "绑定端口成功...");
        StatusInfo = "正常运行-监听中";

    }
    else
    {
        PRINT_TO_LOG(Status, "%s: %s", "绑定端口失败！", Node->ErrorInfo);
        StatusInfo = "停止运行-端口占用";
        //TODO: 可以增加关闭该服务器的弹出框
        return;
    }

    /* 开启监听 */
    //TODO: 监听队列数量可配置
    if (Node->Listen())
    {
        PRINT_TO_LOG(Status, "%s", "开始监听连接...");
    }
    else
    {
        PRINT_TO_LOG(Status, "%s: %s", "监听失败！", Node->ErrorInfo);
        delete Node;
        return;
    }

    /* epoll 初始化: 将服务端套接字加入监听列表*/
    m_epfd = epoll_create(256);
    m_Event.data.fd = Node->Socket;
    m_Event.events = EPOLLIN | EPOLLET;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, Node->Socket, &m_Event);

    Node->SetBlock(false); /* 将服务端套接字设置成非阻塞模式 */

    while (Node->Running) /* 服务端主循环 */
    {
        int cnt = epoll_wait(m_epfd, m_Events, 20, 500); /* 阻塞500ms，监听事件 */
        if (cnt <= 0)
            continue;

        /* 处理所有检测到的事件（连接或者消息接收） */
        for (int i = 0; i < cnt; i++) 
        {
            m_Event = m_Events[i];
            if (m_Event.data.fd == Node->Socket) /* 判断是否为服务端事件 */
                ServerEvent(); /* 建立连接 */
            else
                ClientEvent(); /* 消息收发 */
        }
    }
}

ServerThread::~ServerThread()
{
    for (auto& client : m_ConnectedClients) 
        client.second.Send("close", 6);

    Node->Close();
    delete Node;
    //TODO: debug
    /* Node = nullptr; */

}

void ServerThread::ServerEvent()
{
    Message msg;
    while (true)
    {
        Network::TCPNode client = Node->Accept();
        if (client.Socket <= 0) break;

        /* 获取客户端名称 */
        client.Recv(&msg, sizeof(msg));
        client.Name = Message::BitsToString(msg.Data);
        PRINT_TO_LOG(Status, "%s [IP %s] 建立连接...", 
                client.Name.c_str(), client.IP.c_str());

        m_ConnectedClients.insert(std::make_pair(client.Socket, client));

        char recv[1024] = {0};
        sprintf(recv, "\t\t\t\t\t\t\t\t" ICON_FK_BELL "\t\t[%s]\t进入聊天室", client.Name.c_str());
        for (auto& c : m_ConnectedClients) 
            c.second.Send(recv, strlen(recv) + 1);

        m_Event.data.fd = client.Socket;
        m_Event.events = EPOLLIN | EPOLLET;
        epoll_ctl(m_epfd, EPOLL_CTL_ADD, client.Socket, &m_Event);
    }
}

void ServerThread::ClientEvent()
{
    Message msg;
    Network::TCPNode client;
    client = m_ConnectedClients.at(m_Event.data.fd);
    if (client.Recv(&msg, sizeof(msg)) <= 0)
    {
        PRINT_TO_LOG(Status, "%s [%s] 断开连接!", 
                client.Name.c_str(), client.IP.c_str());

        char recv[1024] = {0};
        sprintf(recv, "\t\t\t\t\t\t\t\t" ICON_FK_BELL "\t\t[%s]\t离开聊天室", client.Name.c_str());
        for (auto& c : m_ConnectedClients) 
            c.second.Send(recv, strlen(recv) + 1);

        m_ConnectedClients.erase(m_Event.data.fd);
        epoll_ctl(m_epfd, EPOLL_CTL_DEL, client.Socket, &m_Event);
    }
    else
    {
        if (Message::Decode(msg))
        {
            char recv[1024] = {0};
            sprintf(recv, "<%s " ICON_FK_COMMENTING " > %s", 
                    client.Name.c_str(), Message::BitsToString(msg.Data).c_str());

            PRINT_TO_MSG(Message, "%s[校验通过]", recv);
            for (auto& c : m_ConnectedClients) 
                c.second.Send(recv, strlen(recv) + 1);
        }
        else
            client.Send("校验错误", 13);
    }

}

void ServerThread::UIRenderer(bool *open)
{
    //TODO: 显示客户端连接数，到最大值怎么办?
    if (ImGui::BeginTabItem(Name.c_str(), open, ImGuiTabItemFlags_None))
    {
        if (!StatusInfo.compare("正常运行-监听中"))
            ImGui::PushStyleColor(0, ImVec4(0.4f, 0.8f, 0.2f, 1.f));
        else
            ImGui::PushStyleColor(0, ImVec4(0.8f, 0.4f, 0.2f, 1.f));
        ImGui::BulletText("本地服务器[端口 %d]状态：%s", Node->Port, StatusInfo.c_str());
        ImGui::PopStyleColor();
        ImGui::BulletText("客户端连接数：%d", (int)m_ConnectedClients.size());
        ImGui::EndTabItem();
    }

    MessageLog.Draw(Name.c_str(), open);
    StatusLog.Draw(Name.c_str(), open);
}

}
