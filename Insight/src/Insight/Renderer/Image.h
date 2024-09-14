#pragma once

#include <imgui.h>

#include "DescriptorSet.h"
#include "../Math.h"
#include "Vulkan.h"
#include "Sampler.h"

namespace Insight::Renderer
{
    class CommandBuffer;

    struct ImageInfo
    {
        string Name = "Image";
        uvec3 Extent = {1, 1, 1};
        u32 MipLevels = 1;
        u32 Layers = 1;
        vk::Format Format;
        vk::ImageType Type = vk::ImageType::e2D;
        vk::ImageUsageFlags Usage;
        vk::Filter Filter = vk::Filter::eLinear;
    };

    class Image
    {
    public:
        static Ref<Image> Create(vk::Image vkImage, const ImageInfo& info)
        {
            return CreateRef<Image>(vkImage, info);
        }

        static Ref<Image> Create(const ImageInfo& info)
        {
            return CreateRef<Image>(info);
        }

        Image(vk::Image vkImage, const ImageInfo& info);
        explicit Image(const ImageInfo& info);

        ~Image();

        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::Image GetVkImage() const { return m_VkImage; }
        [[nodiscard]] vk::Format GetFormat() const { return m_Format; }
        [[nodiscard]] vk::ImageUsageFlags GetUsage() const { return m_Usage; }
        [[nodiscard]] vk::Filter GetFilter() const { return m_Filter; }
        [[nodiscard]] uvec3 GetExtent() const { return m_Extent; }
        [[nodiscard]] u32 GetWidth() const { return m_Extent.x; }
        [[nodiscard]] u32 GetHeight() const { return m_Extent.y; }
        [[nodiscard]] u32 GetDepth() const { return m_Extent.z; }
        [[nodiscard]] u32 GetMipLevels() const { return m_MipLevels; }
        [[nodiscard]] u32 GetLayers() const { return m_Layers; }
        [[nodiscard]] bool IsManaged() const { return m_Managed; }

        void SetLayout(const vk::ImageLayout layout) { for (auto& l : m_Layouts) l = layout; }
        void SetLayout(const u32 layer, const vk::ImageLayout layout) { m_Layouts[layer] = layout; }

        void SetLayout(const u32 layer, const u32 layerCount, const vk::ImageLayout layout)
        {
            for (u32 i = layer; i < layer + layerCount; i++)
                m_Layouts[i] = layout;
        }

        [[nodiscard]] vk::ImageLayout GetLayout(u32 layer) const { return m_Layouts[layer]; }

        [[nodiscard]] ImTextureID GetImTextureId();
        [[nodiscard]] ImTextureID GetCubemapFaceImTextureId(u32 face);
        ImTextureID GetCubemapFaceMipImTextureId(u32 face, u32 mip);
        void GenerateMipmaps(const CommandBuffer& commandBuffer);
        Ref<Image> EquirectangularToCubemap();
        void Upload(std::span<u8> span);
        std::span<u8> Download();
        [[nodiscard]] Ref<Image> CreateIrradianceMap();
        Ref<Image> CreatePrefilteredEnvironmentMap();

    private:
        string m_Name;
        uvec3 m_Extent;
        u32 m_MipLevels;
        u32 m_Layers;
        vector<vk::ImageLayout> m_Layouts{};
        vk::Format m_Format;
        vk::ImageType m_Type;
        vk::ImageUsageFlags m_Usage;
        vk::Filter m_Filter;
        bool m_Managed;

        vk::Image m_VkImage;
        VmaAllocationInfo m_VmaAllocationInfo{};
        VmaAllocation m_VmaAllocation = nullptr;

        ImTextureID m_UiTextureID;
        vector<ImTextureID> m_UiCubemapFaceTextureIDs;
        vector<vector<ImTextureID>> m_UiCubemapFaceMipTextureIDs;

        Ref<Sampler> m_UiSampler;
        Ref<ImageView> m_UiImageView;
        vector<Ref<ImageView>> m_UiCubemapFaceImageViews;
        vector<vector<Ref<ImageView>>> m_UiCubemapFaceMipImageViews;
    };
}
