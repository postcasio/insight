#pragma once
#include "Asset.h"
#include "../Renderer/Image.h"
#include "../Renderer/ImageView.h"
#include "../Renderer/Sampler.h"

namespace Insight
{
    enum class TextureType : u32
    {
        Texture2D,
        TextureCubemap
    };

    struct TextureInfo
    {
        std::span<u8> Data;
        string Name;
        TextureType Type = TextureType::Texture2D;
        u32 Width;
        u32 Height;
        u32 Depth = 1;
        vk::Format Format;
        vk::Filter Filter;
        vk::SamplerAddressMode AddressMode;
        u32 MipLevels;
        bool KeepData;
    };

    struct TextureFromImageInfo
    {
        string Name;
        TextureType Type  = TextureType::Texture2D;
        vk::Format Format;
        vk::Filter Filter;
        vk::SamplerAddressMode AddressMode;
        u32 MipLevels;
    };

    u32 CalculateMipLevels(u32 width, u32 height);

    class Texture : public Asset
    {
    public:
        static Ref<Texture> Create(const TextureInfo& info)
        {
            return CreateRef<Texture>(info);
        }

        static Ref<Texture> Create(const Ref<Renderer::Image>& image, const TextureFromImageInfo& info)
        {
            return CreateRef<Texture>(image, info);
        }

        explicit Texture(const TextureInfo& info);
        Texture(const Ref<Renderer::Image>& image, const TextureFromImageInfo& info);
        ~Texture() override;

        [[nodiscard]] Ref<Renderer::Image> GetImage() const { return m_Image; }
        [[nodiscard]] Ref<Renderer::ImageView> GetImageView() const { return m_LayerImageViews[0]; }
        [[nodiscard]] vector<Ref<Renderer::ImageView>>& GetLayerImageViews() { return m_LayerImageViews; }
        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::Filter GetFilter() const { return m_Filter; }
        [[nodiscard]] vk::SamplerAddressMode GetAddressMode() const { return m_AddressMode; }
        [[nodiscard]] Ref<Renderer::Sampler> GetSampler() const { return m_Sampler; }
        [[nodiscard]] std::span<u8> &GetData() { return m_Data.value(); }
        [[nodiscard]] bool HasData() const { return m_Data.has_value(); }
        [[nodiscard]] u32 GetWidth() const { return m_Image->GetExtent().x; }
        [[nodiscard]] u32 GetHeight() const { return m_Image->GetExtent().y; }
        [[nodiscard]] vk::Format GetFormat() const { return m_Image->GetFormat(); }
        [[nodiscard]] u32 GetMipLevels() const { return m_Image->GetMipLevels(); }
        [[nodiscard]] u32 GetMaximumSupportedMipLevels() const { return m_MaximumSupportedMipLevels; }
        [[nodiscard]] AssetType GetType() const override { return AssetType::Texture; }
        [[nodiscard]] Ref<Texture> EquirectangularToCubemap();
        [[nodiscard]] Ref<Renderer::ImageView> GetCubemapImageView() const { return m_CubemapImageView; }
        [[nodiscard]] Ref<Texture> CreateIrradianceMap();
        [[nodiscard]] Ref<Texture> CreatePrefilteredEnvironmentMap();

    private:
        string m_Name;
        TextureType m_Type;
        vk::Filter m_Filter;
        vk::SamplerAddressMode m_AddressMode;
        Ref<Renderer::Image> m_Image;
        vector<Ref<Renderer::ImageView>> m_LayerImageViews;
        Ref<Renderer::ImageView> m_CubemapImageView;
        Ref<Renderer::Sampler> m_Sampler;
        optional<std::span<u8>> m_Data{};
        u32 m_MaximumSupportedMipLevels = 0;
    };
}
