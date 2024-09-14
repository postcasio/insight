#include "Panel.h"

#include <imgui_internal.h>

#include "Widgets.h"

namespace Insight
{
    Panel::Panel(const string& title):
        m_Title(title)
    {
    }

    void Panel::RenderDockWindow()
    {
        ImGuiWindowClass dockSpaceClass;
        Widgets::FillWindowClass(m_DockSpaceClass, &dockSpaceClass);

        Widgets::SetNextWindowClass(m_WindowClass, true);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


        auto open = (Widgets::BeginWindow(m_Title + "##DockSpaceWindow", &m_Open, m_WindowFlags));
        auto size = ImGui::GetContentRegionAvail();
        auto id = ImGui::GetID((m_Title + "DockSpace").c_str());

        ImGui::DockSpace(id, ImVec2(0, 0), open ? ImGuiDockNodeFlags_None : ImGuiDockNodeFlags_KeepAliveOnly,
                         &dockSpaceClass);

        Widgets::EndWindow();

        if (m_FirstRender)
        {
            ImGui::DockBuilderRemoveNode(id);
            ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(id, size);

            m_MainDockspaceId = SetupDockSpace(id);

            ImGui::DockBuilderFinish(id);
        }

        ImGui::PopStyleVar();
    }

    void Panel::OnRender()
    {
        ImGuiWindowClass dockSpaceClass;
        Widgets::FillWindowClass(m_DockSpaceClass, &dockSpaceClass);

        if (!m_Open)
        {
            return;
        }

        ImGuiWindowFlags flags = m_WindowFlags;

        if (m_CreateDockSpaceWindow)
        {
            RenderDockWindow();
            Widgets::SetNextWindowClass(m_DockSpaceClass, false);
            // flags |= ImGuiWindowFlags_NoTitleBar;
        }
        else
        {
            Widgets::SetNextWindowClass(m_WindowClass, true);
        }
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));

        const auto open = (Widgets::BeginWindow(m_Title, &m_Open, flags));

        if (open)
        {
            OnRenderContents();

            OnRenderMainMenu();
        }

        Widgets::EndWindow();

        ImGui::PopStyleVar();

        m_FirstRender = false;

        const auto children = m_Children;
        for (const auto child : children)
        {
            child->OnRender();
        }
    }
}