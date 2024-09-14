#include "ObjectProperties.h"

#include "../../Widgets.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/DirectionalLightComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/NameComponent.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Components/TransformComponent.h"
#include "Insight/Ui/Widgets.h"

#define RENDER_COMPONENT(entity, component) \
    if (entity.HasComponent<component>()) \
    { \
        Render##component(entity); \
    }

namespace Portent::Editors
{
    void ObjectProperties::RenderTransformComponent(Entity entity)
    {
        auto& transform = entity.GetComponent<TransformComponent>();

        if (Insight::Widgets::ExpandingPropertyTreeLabel("Transform"))
        {
            Insight::Widgets::PropertyTreeDragFloat3("Position", &transform.Position.x, 0.1f, -FLT_MAX, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat3("Rotation", &transform.Rotation.x, 0.1f, -FLT_MAX, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat3("Scale", &transform.Scale.x, 0.1f, -FLT_MAX, FLT_MAX);
        }
    }

    void ObjectProperties::RenderCameraComponent(Entity entity)
    {
        auto& camera = entity.GetComponent<CameraComponent>();

        if (Insight::Widgets::ExpandingPropertyTreeLabel("Camera"))
        {
            Insight::Widgets::PropertyTreeDragFloat("Field of View", &camera.FieldOfView, 0.1f, 0.0f, 180.0f);
            Insight::Widgets::PropertyTreeDragFloat("Near", &camera.NearPlane, 0.1f, FLT_MIN, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat("Far", &camera.FarPlane, 0.1f, FLT_MIN, FLT_MAX);
        }
    }

    void ObjectProperties::RenderDirectionalLightComponent(Entity entity)
    {
        auto& directionalLight = entity.GetComponent<DirectionalLightComponent>();

        if (Insight::Widgets::ExpandingPropertyTreeLabel("Directional Light"))
        {
            Insight::Widgets::PropertyTreeColor("Color", &directionalLight.Color);
            Insight::Widgets::PropertyTreeDragFloat("Intensity", &directionalLight.Intensity, 0.1f, 0.0f, FLT_MAX);
        }
    }

    void ObjectProperties::RenderStaticMeshComponent(Entity entity)
    {
        auto& staticMesh = entity.GetComponent<StaticMeshComponent>();
        auto& library = Editor::Instance().GetProject().GetLibrary();

        if (Insight::Widgets::ExpandingPropertyTreeLabel("Static Mesh"))
        {
            AssetDescriptor selectedAsset;

            if (Widgets::PropertyTreeAssetSelector("Mesh", staticMesh.Mesh, AssetType::StaticMesh, m_FileBrowser, &selectedAsset))
            {
                staticMesh.Mesh = library.LoadAsset<StaticMesh>(selectedAsset.Id);
            }
        }
    }

    void ObjectProperties::RenderEnvironmentComponent(Entity entity)
    {
        auto& environment = entity.GetComponent<EnvironmentComponent>().Environment;

        if (Insight::Widgets::ExpandingPropertyTreeLabel("Environment"))
        {
            AssetDescriptor selectedAsset;

            if (Widgets::PropertyTreeAssetSelector("Environment Map", environment->GetEnvironmentMap(), AssetType::Texture, m_FileBrowser, &selectedAsset))
            {
                environment->SetEnvironmentMap(Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(selectedAsset.Id));
            }

            ImGui::PushID("Cubemap");
            Insight::Widgets::PropertyTreeLabel("Cubemap");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            Insight::Widgets::Cubemap(environment->GetEnvironmentCubemap()->GetImage(), {128, 128});
            ImGui::PopID();

            ImGui::PushID("Irradiance map");
            Insight::Widgets::PropertyTreeLabel("Irradiance map");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            Insight::Widgets::Cubemap(environment->GetEnvironmentIrradianceMap()->GetImage(), {128, 128});
            ImGui::PopID();

            ImGui::PushID("Prefiltered map");
            Insight::Widgets::PropertyTreeLabel("Prefiltered map");
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            Insight::Widgets::CubemapMips(environment->GetPrefilteredEnvironmentCubemap()->GetImage(), {128, 128});
            ImGui::PopID();

        }
    }

    void ObjectProperties::OnRenderContents()
    {
        auto selectedEntity = m_PrefabEditor.GetSelectedEntity();

        if (selectedEntity.IsValid())
        {
            auto& nameComponent = selectedEntity.GetComponent<NameComponent>();

            if (Insight::Widgets::BeginPropertyTree(nameComponent.Name))
            {
                Insight::Widgets::PropertyTreeText("Name", nameComponent.Name);

                RENDER_COMPONENT(selectedEntity, TransformComponent);
                RENDER_COMPONENT(selectedEntity, CameraComponent);
                RENDER_COMPONENT(selectedEntity, DirectionalLightComponent);
                RENDER_COMPONENT(selectedEntity, StaticMeshComponent);
                RENDER_COMPONENT(selectedEntity, EnvironmentComponent);

                Insight::Widgets::EndPropertyTree();
            }

        }
    }
}
