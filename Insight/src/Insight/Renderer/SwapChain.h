#pragma once

#include "Framebuffer.h"
#include "../Insight.h"
#include "Vulkan.h"
#include "Image.h"
#include "Semaphore.h"
#include "../Windows/Window.h"
#include "SwapChainSupport.h"

namespace Insight::Renderer
{
    struct SwapChainInfo
    {
        u32 MaxImageCount;
        u32 Width;
        u32 Height;
        vk::Format Format;
    };

    class SwapChain
    {
    public:
        static Unique<SwapChain> Create(const SwapChainInfo& info)
        {
            return CreateUnique<SwapChain>(info);
        }

        void Initialize();
        explicit SwapChain(const SwapChainInfo& info);
        ~SwapChain();

        void RenderExtentChanged();

        [[nodiscard]] u32 GetWidth() const { return m_Width; }
        [[nodiscard]] u32 GetHeight() const { return m_Height; }
        [[nodiscard]] uvec2 GetExtent() const { return { m_Width, m_Height }; }
        [[nodiscard]] vk::Extent2D GetSurfaceExtent(Window &window, SwapChainSupportDetails &swapChainSupport);
        [[nodiscard]] u32 GetMaxImageCount() const { return m_MaxImageCount; }
        [[nodiscard]] u32 GetImageCount() const { return m_ImageCount; }
        [[nodiscard]] u32 GetCurrentImageIndex() const { return m_CurrentImageIndex; }
        [[nodiscard]] vk::Format GetFormat() const { return m_Format; }

        [[nodiscard]] vk::Result AcquireNextImage(Semaphore& semaphore);

        [[nodiscard]] vk::SwapchainKHR GetVkSwapChain() const { return m_VkSwapChain; }
        [[nodiscard]] Ref<Image> GetImage(const u32 index) const { return m_Images[index]; }
        [[nodiscard]] Ref<Image> GetCurrentImage() const { return m_Images[m_CurrentImageIndex]; }

        [[nodiscard]] Ref<ImageView> GetImageView(const u32 index) const { return m_ImageViews[index]; }
        [[nodiscard]] Ref<ImageView> GetCurrentImageView() const { return m_ImageViews[m_CurrentImageIndex]; }


    protected:
    private:
        u32 m_Width;
        u32 m_Height;
        u32 m_MaxImageCount;
        u32 m_ImageCount{};
        u32 m_CurrentImageIndex = 0;
        vk::Format m_Format;
        vk::ColorSpaceKHR m_ColorSpace;
        vk::SwapchainKHR m_VkSwapChain;
        vector<Ref<Image>> m_Images {};
        vector<Ref<ImageView>> m_ImageViews {};

        void Dispose();
    };
}
