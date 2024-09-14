#pragma once
#include "MaterialEditor.h"
#include "../EditorPanel.h"

namespace Portent::Editors {

    class MaterialEditorProperties final : public Panel {
    public:
        explicit MaterialEditorProperties(MaterialEditor &materialEditor) : Panel("Properties##MaterialEditor"), m_MaterialEditor(materialEditor)
        {
            m_WindowClass = "MaterialEditor";
        };

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<MaterialEditor*>(m_Parent);

            m_Title = "Properties##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        ~MaterialEditorProperties() override = default;

        void OnRenderContents() override;

    private:
        MaterialEditor &m_MaterialEditor;
    };

}
