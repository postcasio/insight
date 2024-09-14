#include "Framebuffer.h"

#include "Context.h"
#include "DebugMarkers.h"

namespace Insight::Renderer {
    Framebuffer::~Framebuffer()
    {
        Context::Instance().GetVkDevice().destroyFramebuffer(m_VkFramebuffer);
    }

    Framebuffer::Framebuffer(const FramebufferInfo &info) : m_Name(info.Name), m_Attachments(info.Attachments), m_Extent(info.Width, info.Height)
    {
        vector<vk::ImageView> attachments;

        attachments.reserve(info.Attachments.size());


        for (const auto &attachment : info.Attachments) {
            attachments.push_back(attachment.ImageView.GetVkImageView());
        }

        vk::FramebufferCreateInfo framebufferCreateInfo {
            .renderPass = info.RenderPass.GetVkRenderPass(),
            .attachmentCount = static_cast<u32>(attachments.size()),
            .pAttachments = attachments.data(),
            .width = info.Width,
            .height = info.Height,
            .layers = info.Layers
        };

        m_VkFramebuffer = Context::Instance().GetVkDevice().createFramebuffer(framebufferCreateInfo);

        DebugMarkers::MarkFramebuffer(m_VkFramebuffer, m_Name);
    }
}
