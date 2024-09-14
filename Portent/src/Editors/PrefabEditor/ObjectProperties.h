#pragma once

#include "PrefabEditor.h"
#include "../EditorPanel.h"
#include "../../Controls/FileBrowser.h"

namespace Portent::Editors
{
    class ObjectProperties final : public Panel
    {
    public:
        explicit ObjectProperties(PrefabEditor& sceneEditor) : Panel("Object Properties##PrefabEditor"),
                                                           m_PrefabEditor(sceneEditor)
        {
            m_WindowClass = "PrefabEditor";
        };

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<PrefabEditor*>(m_Parent);

            m_Title = "Object Properties##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        ~ObjectProperties() override = default;

        void RenderTransformComponent(Entity entity);
        void RenderCameraComponent(Entity entity);
        void RenderDirectionalLightComponent(Entity entity);
        void RenderStaticMeshComponent(Entity entity);
        void RenderEnvironmentComponent(Entity entity);

        void OnRenderContents() override;

    private:
        PrefabEditor& m_PrefabEditor;
        Controls::FileBrowser m_FileBrowser {{Editor::Instance().GetProject().GetLibrary().GetDirectory()}};
    };
}
