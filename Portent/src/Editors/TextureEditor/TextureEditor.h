#pragma once

#include "../EditorPanel.h"
#include "Insight/AssetDescriptor.h"
#include "Insight/Assets/Texture.h"


namespace Portent::Editors
{
    class TextureEditorProperties;

    class TextureEditor final : public EditorPanel
    {
        struct TextureMipView
        {
            Ref<Renderer::ImageView> ImageView;
            ImTextureID ImTextureId;
            uvec2 Extent;
        };

    public:
        TextureEditor() : EditorPanel("Texture Editor")
        {
            m_CreateDockSpaceWindow = true;
            m_DockSpaceClass = "TextureEditor" + Uuid::CreateNew().ToString();
        };
        ~TextureEditor() override = default;

        void OnAdded() override;

        void OnRenderContents() override;
        void OnRenderMainMenu() override;
        ImGuiID SetupDockSpace(ImGuiID dockSpaceId) override;

        [[nodiscard]] bool HasTexture() const { return m_Texture != nullptr; }
        [[nodiscard]] Ref<Texture> GetTexture() const { return m_Texture; }
        [[nodiscard]] vk::Filter &GetFilter()  { return m_Filter; }
        [[nodiscard]] vk::Format &GetFormat() { return m_Format; }
        [[nodiscard]] vk::SamplerAddressMode &GetAddressMode()  { return m_AddressMode; }
        [[nodiscard]] u32 &GetMipLevels() { return m_MipLevels; }

        void Refresh();
        void QueueRefresh() { m_RefreshQueued = true; }

        void LoadAsset(AssetHandle id) override;
        void SaveAsset(AssetDescriptor& descriptor) override;
        [[nodiscard]] bool HasAsset() const override { return m_Texture != nullptr; }
        [[nodiscard]] Ref<Asset> GetAsset() const override { return m_Texture; }
        [[nodiscard]] path GetTexturePath() const { return m_TexturePath; }
        [[nodiscard]] AssetHandle GetAssetHandle() const override { return m_Texture->GetHandle(); }

    private:
        json m_TextureJson;
        path m_TexturePath;
        AssetDescriptor m_AssetDescriptor;
        Ref<Texture> m_Texture{};
        vector<TextureMipView> m_MipViews;
        Ref<Renderer::Sampler> m_Sampler;
        u32 m_MipLevels;
        TextureEditorProperties* m_Properties = nullptr;

        vk::Filter m_Filter;
        vk::Format m_Format;
        vk::SamplerAddressMode m_AddressMode;

        ImGuiID m_LeftId;
        ImGuiID m_RightId;

        bool m_RefreshQueued = false;
    };
}
