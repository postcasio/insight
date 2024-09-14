#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    struct RenderPassInfo
    {
        string Name;
        vector<vk::AttachmentDescription> Attachments;
    };

    class RenderPass
    {
    public:
        static Unique<RenderPass> Create(const RenderPassInfo& info)
        {
            return CreateUnique<RenderPass>(info);
        }

        explicit RenderPass(const RenderPassInfo& info);
        ~RenderPass();

        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::RenderPass GetVkRenderPass() const { return m_VkRenderPass; }
        [[nodiscard]] const vector<vk::AttachmentDescription>& GetAttachments() const { return m_Attachments; }

    private:
        string m_Name;
        vk::RenderPass m_VkRenderPass;
        vector<vk::AttachmentDescription> m_Attachments;
    };
}