#pragma once

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>
#include <vulkan/vk_enum_string_helper.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


#include "VulkanMemoryAllocator.h"

#include "../Assert.h"

#define VK_RAW_ASSERT(f, msg)                                                 \
    {                                                                        \
        auto res = (f);                                                  \
        if (res != VK_SUCCESS)                                               \
        {                                                                    \
            Insight::Log::GetEngineLog()                                          \
                .error("Vulkan call failed with {}", string_VkResult(res)); \
            INS_DEBUGBREAK();                                                 \
        }                                                                    \
    }

#define VK_ASSERT(f, msg)                                                 \
    {                                                                        \
        vk::Result res = (f);                                                  \
        if (res != vk::Result::eSuccess)                                               \
        {                                                                    \
            Insight::Log::GetEngineLog()                                          \
                .error("Vulkan call failed with {}", string_VkResult(static_cast<VkResult>(res))); \
            INS_DEBUGBREAK();                                                 \
        }                                                                    \
    }

#define VK_BOOL(b) ((b) ? vk::True : vk::False)