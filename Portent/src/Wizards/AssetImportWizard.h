#pragma once

#include <imgui.h>

#include "../Portent.h"

namespace Portent::Wizards
{
    class AssetImportWizard
    {
    public:
        explicit AssetImportWizard(const string& title) : m_Title(title) { }
        virtual ~AssetImportWizard() = default;

        [[nodiscard]] bool IsOpen() const { return m_Open; }

        void Open(const path& assetPath)
        {
            m_Open = true;
            OnOpen(assetPath);
        }

        void Close()
        {
            ImGui::CloseCurrentPopup();
            OnClose();
        }

        void Render()
        {
            if (m_Open)
            {
                if (ImGui::Begin(m_Title.c_str(), &m_Open, ImGuiWindowFlags_NoDocking))
                {
                    OnRender();
                }

                ImGui::End();
            }
        }

        virtual bool IsSupported(const path& path) = 0;
        virtual void OnOpen(const path& assetPath) { }
        virtual void OnClose() { }
        virtual void OnRender() = 0;

        [[nodiscard]] const string& GetTitle() const { return m_Title; }
        void SetTitle(const string& title) { m_Title = title; }
    protected:
        bool m_Open = false;
        string m_Title;
    };
}