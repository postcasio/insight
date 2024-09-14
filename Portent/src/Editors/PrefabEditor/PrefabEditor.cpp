#include "PrefabEditor.h"

#include <imgui_internal.h>

#include "ObjectProperties.h"
#include "SceneExplorer.h"
#include "../../Editor.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/DirectionalLightComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/PointLightComponent.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Components/TransformComponent.h"
#include "Insight/Geometry/PlaneGeometry.h"
#include "Insight/Renderer/Context.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Editors
{
    PrefabEditor::PrefabEditor(): EditorPanel("Prefab Editor"), m_RenderAreaExtent(1280, 720)
    {
        m_CreateDockSpaceWindow = true;
        m_DockSpaceClass = "PrefabEditor" + Uuid::CreateNew().ToString();

        m_GridTexture = Editor::Instance().GetResources().GetGridTexture();

        m_GridMaterial = Material::Create({
            .Name = "GridBasic",
            .Shader = *Renderer::Shader::Create({
                .Name = "GridBasic",
                .Path = "Editor/Shaders/GridBasic.frag.spv",
                .EntryPoint = "main"
            }),
            .Textures = {m_GridTexture},
            .UniformBufferSize = 0
        });
    }

    ImGuiID PrefabEditor::SetupDockSpace(ImGuiID dockSpaceId)
    {
        ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, 0.75f, &m_LeftId, &m_RightTopId);

        ImGui::DockBuilderDockWindow(m_Title.c_str(), m_LeftId);

        ImGui::DockBuilderSplitNode(m_RightTopId, ImGuiDir_Down, 0.75f, &m_RightBottomId, &m_RightTopId);

        ImGui::DockBuilderDockWindow(m_SceneExplorer->GetTitle().c_str(), m_RightTopId);

        ImGui::DockBuilderDockWindow(m_ObjectProperties->GetTitle().c_str(), m_RightBottomId);

        return m_LeftId;
    }

    void PrefabEditor::LoadAsset(AssetHandle id)
    {
    }

    void PrefabEditor::SaveAsset(AssetDescriptor& descriptor)
    {
    }

    void PrefabEditor::CreateDefaultEntities()
    {
        m_EditorCamera = m_Scene->CreateEntity("Editor Camera");
        m_EditorCamera.AddComponent<CameraComponent>().SetPerspectiveProjection(
            45,
            static_cast<float>(m_RenderAreaExtent.x) / static_cast<float>(m_RenderAreaExtent.y),
            0.1f,
            8192);
        m_EditorCamera.GetComponent<TransformComponent>().Position = {32, 64, 32};
        m_EditorCamera.GetComponent<TransformComponent>().LookAt({0, 0, 0});

        m_EditorPlane = m_Scene->CreateEntity("Editor Plane");

        PlaneGeometry plane({.Width = 4096, .Height = 4096, .WidthSegments = 8, .HeightSegments = 8, .UvScale = {64, 64}});

        auto mesh = StaticMesh::Create({
            .Name = "Editor Plane",
            .Indices = plane.GetIndices(),
            .Vertices = plane.GetVertices(),
            .Submeshes = plane.GetSubmeshes(),
            .Materials = {m_GridMaterial}
        });

        m_EditorPlane.AddComponent<StaticMeshComponent>(mesh);
        m_EditorPlane.GetComponent<TransformComponent>().RotateX(glm::radians(-90.0f));

        const auto defaultLight = m_Scene->CreateEntity("Default Light");
        defaultLight.AddComponent<DirectionalLightComponent>();
        defaultLight.GetComponent<TransformComponent>().Position = {20, 20, 20};

        const auto defaultEnvironment = m_Scene->CreateEntity("Default Environment");
        defaultEnvironment.AddComponent<EnvironmentComponent>();
    }

    void PrefabEditor::OnAdded()
    {
        if (m_AssetDescriptor.Id == AssetHandle::Null)
        {
            m_Scene = Scene::Create();

            CreateDefaultEntities();
        }

        AddPanel(m_SceneExplorer = new SceneExplorer(*this));
        AddPanel(m_ObjectProperties = new ObjectProperties(*this));
    }

    void PrefabEditor::OnRenderContents()
    {
        const auto size = ImGui::GetContentRegionAvail();
        const uvec2 extent = {static_cast<uint32_t>(size.x), static_cast<uint32_t>(size.y)};

        if (extent.x <= 0 || extent.y <= 0)
        {
            return;
        }

        if (m_RenderAreaExtent != extent)
        {
            m_RenderAreaExtent = extent;

            m_Renderer.SetExtent(m_RenderAreaExtent);
            m_Renderer.CreateResources();

            m_RenderImage = Renderer::Image::Create({
                .Name = "Prefab Editor Render",
                .Extent = {m_RenderAreaExtent.x, m_RenderAreaExtent.y, 1},
                .Format = vk::Format::eR16G16B16A16Sfloat,
                .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc |
                vk::ImageUsageFlagBits::eSampled,
                .Filter = vk::Filter::eLinear,
            });

            m_RenderImageView = Renderer::ImageView::Create({
                .Image = *m_RenderImage,
                .Name = "Prefab Editor Render View",
            });

            m_EditorCamera.GetComponent<CameraComponent>().SetPerspectiveProjection(
                45,
                static_cast<float>(m_RenderAreaExtent.x) / static_cast<float>(m_RenderAreaExtent.y),
                0.1f,
                8192);
        }

        m_Renderer.RenderScene({
            .CommandBuffer = Renderer::Context::Instance().GetCurrentFrame().GetCommandBuffer(),
            .Camera = m_EditorCamera,
            .Scene = *m_Scene,
            .Target = *m_RenderImage,
            .TargetView = *m_RenderImageView
        });

        Widgets::Image(m_RenderImage, size);
    }
}
