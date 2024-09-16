#pragma once

#include "PrefabEditor.h"
#include "../EditorPanel.h"
#include "../../Controls/FileBrowser.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/DirectionalLightComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/PointLightComponent.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Editors
{

    struct AddComponentHandler
    {
        string Name;
        std::function<void(Entity)> Handler;
    };

    static inline vector<AddComponentHandler> s_AddComponentHandlers {
            {"DirectionalLight", [](Entity entity) { entity.AddComponent<DirectionalLightComponent>(); }},
            {"PointLight", [](Entity entity) { entity.AddComponent<PointLightComponent>(); }},
            {"StaticMesh", [](Entity entity) { entity.AddComponent<StaticMeshComponent>(); }},
            {"Camera", [](Entity entity) { entity.AddComponent<CameraComponent>(); }},
            {"Environment", [](Entity entity) { entity.AddComponent<EnvironmentComponent>(); }},
        };

    static inline const vector<Insight::Widgets::ComboOption<i32>> s_AddComponentOptions = {
        {0, "DirectionalLight"},
        {1, "PointLight"},
        {2, "StaticMesh"},
        {3, "Camera"},
        {4, "Environment"}
    };

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
