#pragma once
#include "TextureEditor.h"
#include "../EditorPanel.h"


namespace Portent::Editors {

    class TextureEditorProperties final : public Panel {
    public:
        explicit TextureEditorProperties(TextureEditor &textureViewer) : Panel("Properties##TextureEditor"), m_TextureEditor(textureViewer)
        {

        };

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<TextureEditor*>(m_Parent);

            m_Title = "Properties##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        ~TextureEditorProperties() override = default;

        void OnRenderContents() override;

    private:
        TextureEditor &m_TextureEditor;
    };

}
