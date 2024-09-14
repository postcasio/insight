#pragma once
#include "../EditorPanel.h"
#include "Insight/Entity.h"
#include "Insight/Math.h"
#include "Insight/Assets/Material.h"
#include "Insight/Scene.h"
#include "Insight/Assets/Texture.h"
#include "Insight/Assets/Prefab.h"
#include "Insight/Renderer/SceneRenderer.h"

namespace Portent::Editors
{
    class ObjectProperties;
    class SceneExplorer;

    class PrefabEditor final : public EditorPanel
    {
    public:
        PrefabEditor();;
        ~PrefabEditor() override = default;

        ImGuiID SetupDockSpace(ImGuiID dockSpaceId) override;

        void LoadAsset(AssetHandle id) override;
        void SaveAsset(AssetDescriptor& descriptor) override;

        [[nodiscard]] bool HasAsset() const override { return m_Prefab != nullptr; }
        [[nodiscard]] Ref<Asset> GetAsset() const override { return m_Prefab; }
        [[nodiscard]] Ref<Prefab> GetPrefab() { return m_Prefab; }
        [[nodiscard]] AssetHandle GetAssetHandle() const override { return m_Prefab->GetHandle(); }
        void CreateDefaultEntities();
        void OnAdded() override;
        void OnRenderContents() override;
        [[nodiscard]] Ref<Scene> GetScene() const { return m_Scene; }

        void SelectEntity(const Entity entity) { m_SelectedEntity = entity; }
        void DeselectEntity() { m_SelectedEntity = {}; }
        [[nodiscard]] Entity GetSelectedEntity() const { return m_SelectedEntity; }
        [[nodiscard]] bool IsEntitySelected() const { return m_SelectedEntity.IsValid(); }

    private:
        ImGuiID m_LeftId{};
        ImGuiID m_RightTopId{};
        ImGuiID m_RightBottomId{};

        AssetDescriptor m_AssetDescriptor{};

        SceneExplorer *m_SceneExplorer = nullptr;
        ObjectProperties *m_ObjectProperties = nullptr;

        Ref<Prefab> m_Prefab;
        Ref<Scene> m_Scene;
        Entity m_EditorCamera;
        Entity m_EditorPlane;
        uvec2 m_RenderAreaExtent;
        Renderer::SceneRenderer m_Renderer {vk::Format::eR16G16B16A16Sfloat};

        Ref<Renderer::Image> m_RenderImage;
        Ref<Renderer::ImageView> m_RenderImageView;

        Ref<Texture> m_GridTexture;
        Ref<Material> m_GridMaterial;

        Entity m_SelectedEntity{};
    };
}
