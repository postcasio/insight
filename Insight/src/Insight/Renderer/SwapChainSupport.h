#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR Capabilities;
        vector<vk::SurfaceFormatKHR> Formats;
        vector<vk::PresentModeKHR> PresentModes;
    };
}