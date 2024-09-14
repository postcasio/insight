#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    class Fence
    {
    public:
        static Unique<Fence> Create(const string& name, vk::FenceCreateFlags flags = {})
        {
            return CreateUnique<Fence>(name, flags);
        }

        explicit Fence(const string& name, vk::FenceCreateFlags flags);
        ~Fence();

        void Wait() const;
        void Reset() const;

        [[nodiscard]] vk::Fence GetVkFence() const { return m_VkFence; }

    private:
        string m_Name;
        vk::Fence m_VkFence;
    };
}
