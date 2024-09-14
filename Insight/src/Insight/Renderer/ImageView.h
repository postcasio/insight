#pragma once

#include "Vulkan.h"

namespace Insight::Renderer {

    class Image;

struct ImageViewInfo
{
    string Name;
    Image &Image;

    vk::ImageViewType ViewType = vk::ImageViewType::e2D;
    u32 BaseMipLevel = 0;
    u32 MipLevels = 1;
    u32 BaseArrayLayer = 0;
    u32 ArrayLayers = 1;
    vk::ComponentMapping Components = { vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity };
    vk::ImageViewCreateFlags Flags = {};
};

class ImageView {
public:
    static Ref<ImageView> Create(const ImageViewInfo &info)
    {
        return CreateRef<ImageView>(info);
    }

    ~ImageView();
    explicit ImageView(const ImageViewInfo &info);

    [[nodiscard]] string GetName() const { return m_Name; }
    [[nodiscard]] vk::ImageView GetVkImageView() const { return m_VkImageView; }
    [[nodiscard]] vk::Format GetFormat() const { return m_Format; }
    [[nodiscard]] Image& GetImage() const { return m_Image; }

private:
    string m_Name;
    vk::Format m_Format;
    vk::ImageView m_VkImageView;
    Image& m_Image;
};

}