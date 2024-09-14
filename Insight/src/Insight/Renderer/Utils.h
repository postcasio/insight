#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    class Utils
    {
    public:
        static bool IsDepthFormat(vk::Format format);
        static bool IsStencilFormat(vk::Format format);
        static vk::ImageLayout GetFormatAttachmentImageLayout(vk::Format format);
        static vk::ImageAspectFlags GetAspectFlags(vk::Format format);
        static vk::AccessFlags GetAccessMask(vk::ImageLayout imageLayout);
        static vk::PipelineStageFlags GetDestinationStageMask(vk::ImageLayout imageLayout);
        static vk::PipelineStageFlags GetSourceStageMask(vk::ImageLayout imageLayout);
    };
}