#include "ObjectProperties.h"

#include "../../Widgets.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/DirectionalLightComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/NameComponent.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Components/TransformComponent.h"
#include "Insight/Ui/FontAwesome6.h"
#include "Insight/Ui/Widgets.h"

#define RENDER_COMPONENT(entity, component) \
    if (entity.HasComponent<component>()) \
    { \
        Render##component(entity); \
    }

#define RENDER_REMOVE_COMPONENT(component) \
    if (entity.HasComponent<component>()) \
    { \
        ImGui::TableSetColumnIndex(1); \
        if (Insight::Widgets::SmallButton(ICON_FA_X)) \
        { \
            entity.RemoveComponent<component>(); \
            return; \
        } \
    }

#define RENDER_COMPONENT_IMPL(component, label) \
    void ObjectProperties::Render##component(Entity entity) \
    { \
        auto expanded = Insight::Widgets::ExpandingPropertyTreeLabel(label); \
        if (expanded)

#define RENDER_REMOVEABLE_COMPONENT_IMPL(component, label) \
    void ObjectProperties::Render##component(Entity entity) \
    { \
        auto expanded = Insight::Widgets::ExpandingPropertyTreeLabel(label); \
        RENDER_REMOVE_COMPONENT(component); \
        if (expanded)

#define RENDER_COMPONENT_IMPL_END }

namespace Portent::Editors
{
    RENDER_COMPONENT_IMPL(TransformComponent, "Transform")
        {
            auto transform = entity.GetComponent<TransformComponent>();

            Insight::Widgets::PropertyTreeDragFloat3("Position", &transform.Position.x, 0.1f, -FLT_MAX, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat3("Rotation", &transform.Rotation.x, 0.1f, -FLT_MAX, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat3("Scale", &transform.Scale.x, 0.1f, -FLT_MAX, FLT_MAX);
        }
    RENDER_COMPONENT_IMPL_END

    RENDER_REMOVEABLE_COMPONENT_IMPL(CameraComponent, "Camera")
        {
            auto& camera = entity.GetComponent<CameraComponent>();

            Insight::Widgets::PropertyTreeDragFloat("Field of View", &camera.FieldOfView, 0.1f, 0.0f, 180.0f);
            Insight::Widgets::PropertyTreeDragFloat("Near", &camera.NearPlane, 0.1f, FLT_MIN, FLT_MAX);
            Insight::Widgets::PropertyTreeDragFloat("Far", &camera.FarPlane, 0.1f, FLT_MIN, FLT_MAX);
        }
    RENDER_COMPONENT_IMPL_END

    RENDER_REMOVEABLE_COMPONENT_IMPL(DirectionalLightComponent, "Directional Light")
        {
            auto& directionalLight = entity.GetComponent<DirectionalLightComponent>();

            Insight::Widgets::PropertyTreeColor("Color", &directionalLight.Color);
            Insight::Widgets::PropertyTreeDragFloat("Intensity", &directionalLight.Intensity, 0.1f, 0.0f, FLT_MAX);
        }
    RENDER_COMPONENT_IMPL_END

    RENDER_REMOVEABLE_COMPONENT_IMPL(StaticMeshComponent, "Static Mesh")
        {
            auto& staticMesh = entity.GetComponent<StaticMeshComponent>();
            auto& library = Editor::Instance().GetProject().GetLibrary();
            AssetDescriptor selectedAsset;

            if (Widgets::PropertyTreeAssetSelector("Mesh", staticMesh.Mesh, AssetType::StaticMesh, m_FileBrowser,
                                                   &selectedAsset))
            {
                staticMesh.Mesh = library.LoadAsset<StaticMesh>(selectedAsset.Id);
            }

            if (staticMesh.Mesh)
            {
                int updateMaterialIndex = -1;
                Ref<Material> updateMaterial{};

                auto& materials = staticMesh.Mesh->GetMaterials();
                for (auto i = 0; i < materials.size(); i++)
                {
                    auto material = materials[i];

                    AssetDescriptor selectedMaterial;

                    if (Widgets::PropertyTreeAssetSelector(fmt::format("Material {0}", i), material,
                                                           AssetType::Material, m_FileBrowser, &selectedMaterial))
                    {
                        material = library.LoadAsset<Material>(selectedMaterial.Id);
                        updateMaterialIndex = i;
                        updateMaterial = material;
                    }
                }

                if (updateMaterialIndex != -1)
                {
                    staticMesh.Mesh->SetMaterial(updateMaterialIndex, updateMaterial);
                }
            }
        }
    RENDER_COMPONENT_IMPL_END

    RENDER_REMOVEABLE_COMPONENT_IMPL(EnvironmentComponent, "Environment")
        {
            auto& environment = entity.GetComponent<EnvironmentComponent>().Environment;


            AssetDescriptor selectedAsset;

            if (Widgets::PropertyTreeAssetSelector("Environment Map", environment->GetEnvironmentMap(),
                                                   AssetType::Texture, m_FileBrowser, &selectedAsset))
            {
                environment->SetEnvironmentMap(
                    Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(selectedAsset.Id));
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
    RENDER_COMPONENT_IMPL_END

    void ObjectProperties::OnRenderContents()
    {
        auto selectedEntity = m_PrefabEditor.GetSelectedEntity();
        static i32 selectedComponent = 0;

        if (selectedEntity.IsValid())
        {
            auto& nameComponent = selectedEntity.GetComponent<NameComponent>();

            Insight::Widgets::Combo("Add Component", &selectedComponent, s_AddComponentOptions.data(),
                                    s_AddComponentOptions.size());

            ImGui::SameLine();

            if (Insight::Widgets::SmallButton(ICON_FA_PLUS))
            {
                s_AddComponentHandlers[selectedComponent].Handler(selectedEntity);
            }

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
