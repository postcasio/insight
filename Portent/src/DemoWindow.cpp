#include "DemoWindow.h"

#include <imgui.h>

namespace Portent
{
    void DemoWindow::OnRender()
    {
        ImGuiWindowClass windowClass;
        windowClass.ClassId = ImGui::GetID("Global");
        windowClass.DockingAllowUnclassed = false;

        ImGui::SetNextWindowClass(&windowClass);
        ImGui::ShowDemoWindow();
    }
}
