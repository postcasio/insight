#pragma once

#include "../Math.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "Vulkan.h"

namespace Insight::Renderer
{
    struct FramebufferAttachmentInfo
    {
        ImageView& ImageView;
    };

    struct FramebufferInfo
    {
        string Name;
        RenderPass& RenderPass;
        vector<FramebufferAttachmentInfo> Attachments = {};
        u32 Width;
        u32 Height;
        u32 Layers = 1;
    };

    class Framebuffer
    {
    public:
        static Unique<Framebuffer> Create(const FramebufferInfo& info)
        {
            return CreateUnique<Framebuffer>(info);
        }

        ~Framebuffer();
        explicit Framebuffer(const FramebufferInfo& info);

        [[nodiscard]] const string& GetName() const { return m_Name; }
        [[nodiscard]] vk::Framebuffer GetVkFramebuffer() const { return m_VkFramebuffer; }
        [[nodiscard]] const vector<FramebufferAttachmentInfo>& GetAttachments() const { return m_Attachments; }
        [[nodiscard]] uvec2 GetExtent() const { return m_Extent; }

    private:
        string m_Name;
        vector<FramebufferAttachmentInfo> m_Attachments;
        vk::Framebuffer m_VkFramebuffer;
        uvec2 m_Extent{};
    };
}
