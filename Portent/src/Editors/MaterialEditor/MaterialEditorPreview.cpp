#include "MaterialEditorPreview.h"

#include <imgui_internal.h>

#include "Insight/Time.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/DirectionalLightComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/PointLightComponent.h"
#include "Insight/Components/TransformComponent.h"
#include "Insight/Geometry/BoxGeometry.h"
#include "Insight/Geometry/SphereGeometry.h"
#include "Insight/Renderer/Command.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Editors {
    MaterialEditorPreview::MaterialEditorPreview(MaterialEditor& materialEditor): Panel("Preview##MaterialEditor"),
        m_MaterialEditor(materialEditor)
    {
        m_WindowClass = "MaterialEditor";
        m_WindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        constexpr uvec2 extent = {512, 512};

        m_Extent = {extent.x, extent.y};


        m_CameraEntity = m_PreviewScene.CreateEntity("Camera");
        m_CameraEntity.AddComponent<CameraComponent>().SetPerspectiveProjection(
            45,
            m_Extent.x / m_Extent.y,
            0.1f,
            8192);
        m_CameraEntity.GetComponent<TransformComponent>().Position = {0, 0, 14};
        m_CameraEntity.GetComponent<TransformComponent>().LookAt({0, 0, 0});
        m_LightEntity = m_PreviewScene.CreateEntity("Light");
        auto& light = m_LightEntity.AddComponent<DirectionalLightComponent>();
        light.Intensity = 0.0f;
        light.Color = {1.0f, 1.0f, 1.0f};
        m_LightEntity.GetComponent<TransformComponent>().Position = {10, 0, 0};
        m_EnvironmentEntity = m_PreviewScene.CreateEntity("Environment");
        m_EnvironmentEntity.AddComponent<EnvironmentComponent>().Environment->SetEnvironmentCubemap(Editor::Instance().GetResources().GetHDRITextures()[0]);

        SphereGeometry geometry({
                    .Radius = 4,
                    .WidthSegments = 64,
                    .HeightSegments = 64,
                });

        BoxGeometry boxGeometry({
            .Width = 4,
            .Height = 4,
            .Depth = 4,
        });

        const auto staticMesh = StaticMesh::Create({
            .Name = "Preview",
            .Vertices = geometry.GetVertices(),
            .Indices = geometry.GetIndices(),
            .Submeshes = geometry.GetSubmeshes(),
            .Materials = {
                m_MaterialEditor.GetMaterial()
            },
            .KeepData = false
        });

        m_PreviewEntity = m_PreviewScene.CreateEntity("Preview");
        m_PreviewEntity.AddComponent<StaticMeshComponent>(staticMesh);


        m_CameraController.SetCamera(m_CameraEntity);
        m_CameraController.SetFocusedEntity(m_PreviewEntity);

        CreateResources();
    }

    void MaterialEditorPreview::CreateResources()
    {
        m_SceneRenderer.SetExtent({m_Extent.x, m_Extent.y});
        m_SceneRenderer.CreateResources();

        m_PreviewImage = Renderer::Image::Create({
            .Name = "MaterialEditorPreview",
            .Extent = {m_Extent.x, m_Extent.y, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Type = vk::ImageType::e2D,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eLinear,
            .MipLevels = 1,
            .Layers = 1
        });

        m_PreviewImageView = Renderer::ImageView::Create({
            .Name = "MaterialEditorPreview",
            .Image = *m_PreviewImage
        });

        m_CameraEntity.GetComponent<CameraComponent>().SetPerspectiveProjection(
            45,
            m_Extent.x / m_Extent.y,
            0.1f,
            8192);

    }

    void MaterialEditorPreview::UpdateModel()
    {
        m_PreviewEntity.GetComponent<StaticMeshComponent>().Mesh->SetMaterial(0, m_MaterialEditor.GetMaterial());
    }

    void MaterialEditorPreview::OnRenderContents()
    {
        const auto size = ImGui::GetContentRegionAvail();
        const auto pos = ImGui::GetCursorPos();

        if (size.x <= 0 || size.y <= 0)
        {
            return;
        }

        if (size.x != m_Extent.x || size.y != m_Extent.y)
        {
            m_ExtentChanging = true;
            m_Extent = size;
        }
        else if (m_ExtentChanging)
        {
            CreateResources();

            m_ExtentChanging = false;
        }

        // INS_CLIENT_INFO("Time: {0}", Time::GetFrameTime());

        m_SceneRenderer.RenderScene({
            .CommandBuffer = Renderer::Context::Instance().GetCurrentFrame().GetCommandBuffer(),
            .Target = *m_PreviewImage,
            .TargetView = *m_PreviewImageView,
            .Scene = m_PreviewScene,
            .Camera = m_CameraEntity
        });


        ImVec2 imageExtent = {static_cast<float>(m_PreviewImage->GetExtent().x), static_cast<float>(m_PreviewImage->GetExtent().y)};

        if (size.x < size.y)
        {
            imageExtent.y = size.x * imageExtent.y / imageExtent.x;
            imageExtent.x = size.x;
        }
        else
        {
            imageExtent.x = size.y * imageExtent.x / imageExtent.y;
            imageExtent.y = size.y;
        }

        Widgets::ImageCentered(m_PreviewImage, {pos.x + size.x / 2, pos.y + size.y / 2}, imageExtent);

        ImGui::SetItemUsingMouseWheel();

        const auto hovered = ImGui::IsWindowHovered();

        if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && hovered)
        {
            m_PreviewActive = true;
        }
        else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            m_PreviewActive = false;
        }

        if (m_PreviewActive && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            const auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

            m_CameraController.RotateLeft(delta.x * 0.0001);
            m_CameraController.RotateUp(delta.y * 0.0001);
        }


        if (hovered)
        {
            const auto delta = ImGui::GetIO().MouseWheel;

            m_CameraController.Zoom(delta * 0.1);
        }
    }

    void MaterialEditorPreview::OnUpdate(float delta)
    {
        m_CameraController.Update(delta);

        Panel::OnUpdate(delta);
    }

    void MaterialEditorPreview::SetEnvironmentCubemap(const Ref<Texture>& map)
    {
        m_EnvironmentEntity.GetComponent<EnvironmentComponent>().Environment->SetEnvironmentCubemap(map);
    }
}
