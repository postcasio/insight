#pragma once

#include <utility>

#include "ImGui.h"
#include "../Insight.h"
#include "../Scripting/JSEngine.h"

namespace Insight
{
    class Panel
    {
    public:
        explicit Panel(const string& title);


        virtual ~Panel()
        {
            ClearPanels();
        }

        virtual void OnAdded()
        {
        }

        void SetWindowClass(const string& newClass)
        {
            m_WindowClass = newClass;
        }

        virtual void OnRemoved() {}

        virtual void OnRender();

        virtual void OnUpdate(float delta)
        {
            for (const auto panel : m_Children)
            {
                panel->OnUpdate(delta);
            }
        }

        virtual void OnRenderContents() {}

        virtual void OnRenderMainMenu() {}

        virtual string GetWindowId() { return m_Title; }

        void AddPanel(Panel* panel)
        {
            m_Children.push_back(panel);
            panel->m_Parent = this;
            panel->OnAdded();
        }

        void RemovePanel(Panel* panel)
        {
            panel->OnRemoved();
            std::erase(m_Children, panel);

            delete panel;
        }

        void ClearPanels()
        {
            for (const auto panel : m_Children)
            {
                panel->OnRemoved();
                delete panel;
            }

            m_Children.clear();
        }

        virtual ImGuiID SetupDockSpace(ImGuiID dockSpaceId)
        {
            return dockSpaceId;
        }

        [[nodiscard]] virtual string GetTitle() const { return m_Title; }
        [[nodiscard]] bool IsOpen() const { return m_Open; }
        [[nodiscard]] string GetDockSpaceClass() const { return m_DockSpaceClass; }

        virtual void ScriptTrace(JSTracer* tracer);

    protected:
        string m_WindowClass = "Global";
        string m_Title = "Untitled";

        bool m_CreateDockSpaceWindow = false;
        string m_DockSpaceClass = "Global";
        ImGuiWindowFlags m_WindowFlags = ImGuiWindowFlags_None;

        bool m_Open = true;

        vector<Panel *> m_Children;

        Panel* m_Parent = nullptr;
    private:
        bool m_FirstRender = true;
        void RenderDockWindow();
        ImGuiID m_MainDockspaceId = 0;
    };
}