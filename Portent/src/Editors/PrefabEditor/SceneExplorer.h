#pragma once

#include "PrefabEditor.h"
#include "../EditorPanel.h"

namespace Portent::Editors
{
    class SceneExplorer final : public Panel
    {
    public:
        explicit SceneExplorer(PrefabEditor& sceneEditor) : Panel("Explorer##PrefabEditor"),
                                                           m_PrefabEditor(sceneEditor)
        {
            m_WindowClass = "PrefabEditor";
        };

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<PrefabEditor*>(m_Parent);

            m_Title = "Explorer##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        ~SceneExplorer() override = default;

        void RenderObject(Entity entity, Scene& scene);
        void OnRenderContents() override;

    private:
        PrefabEditor& m_PrefabEditor;
    };
}
