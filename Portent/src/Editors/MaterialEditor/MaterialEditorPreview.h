#pragma once

#include "MaterialEditor.h"
#include "../../OrbitCameraController.h"
#include "Insight/Entity.h"
#include "Insight/Assets/Material.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Renderer/SceneRenderer.h"
#include "Insight/Renderer/Vertex3D.h"

namespace Portent::Editors
{
    class MaterialEditorPreview final : public Panel
    {
    public:
        explicit MaterialEditorPreview(MaterialEditor& materialEditor);
        void CreateResources();
        void UpdateModel();

        ~MaterialEditorPreview() override = default;

        void OnRenderContents() override;
        void OnUpdate(float delta) override;

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<MaterialEditor*>(m_Parent);

            m_Title = "Preview##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        void SetEnvironmentCubemap(const Ref<Texture>& map);

    private:
        MaterialEditor& m_MaterialEditor;

        Scene m_PreviewScene;
        Entity m_CameraEntity;
        Entity m_PreviewEntity;
        Entity m_LightEntity;
        Entity m_EnvironmentEntity;

        OrbitCameraController m_CameraController{};

        Ref<Renderer::Image> m_PreviewImage;
        Ref<Renderer::ImageView> m_PreviewImageView;

        Renderer::SceneRenderer m_SceneRenderer{vk::Format::eR16G16B16A16Sfloat};

        ImVec2 m_Extent;
        bool m_ExtentChanging = false;
        bool m_PreviewActive = false;
    };
}
