#include "Insight/Ui/ImGui.h"
#include "DockingHost.h"

#include "EditorApplication.h"

namespace Portent
{
    void DockingHost::OnRender()
    {
        ImGuiWindowClass windowClass;
        windowClass.ClassId = ImGui::GetID(m_WindowClass.c_str());
        windowClass.DockingAllowUnclassed = false;

        ImGuiWindowClass dockSpaceClass;
        dockSpaceClass.ClassId = ImGui::GetID(m_DockSpaceClass.c_str());
        dockSpaceClass.DockingAllowUnclassed = false;

        auto &io = ImGui::GetIO();

        ImGui::SetNextWindowPos({0, ImGui::GetCursorPos().y});
        ImGui::SetNextWindowSize({io.DisplaySize.x, io.DisplaySize.y - ImGui::GetCursorPos().y});

        ImGui::SetNextWindowClass(&windowClass);

        ImGui::Begin("DockingHost", &m_Open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

        ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode, &dockSpaceClass);

        ImGui::End();

        for (auto& child : m_Children)
        {
            child->OnRender();
        }
    }
} // Portent
