#pragma once

#include "Vulkan.h"

namespace Insight::Renderer {

class CommandPool {
public:
    static Unique<CommandPool> Create()
    {
        return CreateUnique<CommandPool>();
    }

    [[nodiscard]] vk::CommandPool GetVkCommandPool() const { return m_VkCommandPool; }

    CommandPool();
    ~CommandPool();
protected:


private:

    vk::CommandPool m_VkCommandPool;
};

}