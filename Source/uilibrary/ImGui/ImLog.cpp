#include "ImLog.h"
#include "imgui.h"

#include "IconsFontAwesome.h"

ImLog::ImLog()
{
    Name = ICON_FK_BOMB;
    AutoScroll = true;
    Clear();
}
ImLog::ImLog(std::string name)
    : Name(name)
{
    AutoScroll = true;
    Clear();
}

void    ImLog::Clear()
{
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}

void    ImLog::Draw(const char* title, bool* p_open)
{
    if (ImGui::BeginTabItem(title, p_open, ImGuiTabItemFlags_None))
    {

        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
        ImGui::BeginChild(Name.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionMax().y * 0.4f), true, ImGuiWindowFlags_HorizontalScrollbar);

        ImGui::Text("%s", Name.c_str());
        ImGui::SameLine();
        if (ImGui::Button(ICON_FK_TRASH_O))
        {
            this->Clear();
        }
        ImGui::Separator();


        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step())
        {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                /* ImGui::PushStyleColor(0, ImVec4(0.4f, 0.8f, 0.2f, 1.f)); */
                ImGui::TextUnformatted(line_start, line_end);
                /* ImGui::PopStyleColor(); */
            }
        }
        clipper.End();
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::EndTabItem();

    }
}
