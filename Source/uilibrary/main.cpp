#include "Application.h"
#include "imgui.h"

#include "ImGui/ImLog.h"

#include <iostream>


void func()
{
    // UI
    ImGui::ShowDemoWindow();

    ImLog myLog;
    myLog.Draw("hello");
}

int main()
{
    Application app;

    app.Init();

    app.Run(func);

    return 0;
}
