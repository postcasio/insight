#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    class Semaphore
    {
    public:
        static Unique<Semaphore> Create(const string& name, vk::SemaphoreCreateFlags flags = {})
        {
            return CreateUnique<Semaphore>(name, flags);
        }

        explicit Semaphore(const string& name, vk::SemaphoreCreateFlags flags);
        ~Semaphore();

        [[nodiscard]] vk::Semaphore GetVkSemaphore() const { return m_VkSemaphore; }

    private:
        string m_Name;
        vk::Semaphore m_VkSemaphore;
    };
}
