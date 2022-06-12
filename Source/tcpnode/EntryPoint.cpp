#include "EntryPoint.h"

static std::string hints = "点击 " ICON_FK_PLUS_SQUARE " 创建";

void UI();

int main()
{
    hints += name;

    Application app(600, 950); // 创建应用程序（封装UI库提供的界面demo）
    app.Init();
    app.Run(UI); // 客户端（服务端）页面的布局、组件以函数指针UI的形式传入主循环中进行显示

    // 释放未关闭的客户端（服务端）线程资源
    for (auto& node : TCPThreads) // 线程节点以键值对的形式保存在std::unordered_map（{端口号：线程对象指针}）
    {
        if (node.second)
        {
            if (node.second->Node)
                node.second->Node->Running = false;
            delete node.second;
        }
    }
    return 0;
}


void UI()
{
    ImGui::Begin(name.c_str());
    static ImVector<int> active_tabs;
    static int next_tab_id = 0;

    static bool show_trailing_button = true;
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | 
                                            ImGuiTabBarFlags_Reorderable | 
                                            ImGuiTabBarFlags_FittingPolicyResizeDown |
                                            ImGuiTabBarFlags_TabListPopupButton;
    //创建tab栏
    if (ImGui::BeginTabBar("TagBar", tab_bar_flags))
    {
        if (show_trailing_button)
            if (ImGui::TabItemButton(ICON_FK_PLUS_SQUARE, ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
                newThread = true;

        if (newThread) //创建新的线程，打开一个弹出框
            newTCPThread(&TCPThreads);

        if (!active_tabs.Size && !TCPThreads.size()) // 显示提示信息 
        {
            ImGui::SetCursorPosX( (ImGui::GetWindowWidth() - ImGui::CalcTextSize(hints.c_str()).x) / 2.f);
            ImGui::SetCursorPosY( (ImGui::GetWindowHeight() - ImGui::CalcTextSize(hints.c_str()).y) / 2.f);
            ImGui::Text("%s", hints.c_str());
        }

        for (auto& thread : TCPThreads) 
        {
            bool open = true;
            if (thread.second)
            {
                thread.second->UIRenderer(&open);
                if (!open)
                {
                    if (thread.second->Node)
                        thread.second->Node->Running = false;
                    TCPThreads.erase(thread.second->Name);
                    delete thread.second;
                    break;
                }
            }
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

}
