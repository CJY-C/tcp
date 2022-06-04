#include "MyComponents.h"
#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <cstring>

void CheckOrCancel(std::function<void()> check_func, std::function<void()> cancel_func)
{
    if (ImGui::Button(ICON_FK_CHECK_CIRCLE, ImVec2(ImGui::GetContentRegionAvail().x / 2 - 8, 0))) 
    { 
        check_func();
        ImGui::CloseCurrentPopup(); 
    }
    ImGui::SetItemDefaultFocus();

    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x + 8);

    if (ImGui::Button(ICON_FK_TIMES_CIRCLE, ImVec2(ImGui::GetContentRegionAvail().x, 0))) 
    { 
        cancel_func();
        ImGui::CloseCurrentPopup(); 
    }
}

void InputText(const char* title, unsigned short width, std::string& tag, const uint32_t maxLength)
{
    char* name = (char*)malloc(maxLength * sizeof(char));
    memset(name, 0, sizeof(*name));
    ImGui::Text("%s", title);

    ImGui::SameLine();
    ImGui::PushItemWidth(width);
    char label[32] = "##";
    strcat(label, title);

    if(ImGui::InputTextWithHint(label, tag.c_str(), name, maxLength))
    {
        tag = name;
    }
    ImGui::PopItemWidth();
}
