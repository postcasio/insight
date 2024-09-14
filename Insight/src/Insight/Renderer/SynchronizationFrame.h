#pragma once

#include "CommandBuffer.h"
#include "Fence.h"
#include "Semaphore.h"
#include "Vulkan.h"

namespace Insight::Renderer
{
    class SynchronizationFrame
    {
    public:
        SynchronizationFrame();

        [[nodiscard]] Semaphore& GetImageAvailableSemaphore() const { return *m_ImageAvailableSemaphore; }
        [[nodiscard]] Semaphore& GetRenderFinishedSemaphore() const { return *m_RenderFinishedSemaphore; }
        [[nodiscard]] Fence& GetInFlightFence() const { return *m_InFlightFence; }
        [[nodiscard]] CommandBuffer& GetCommandBuffer() const { return *m_CommandBuffer; }

        template <typename T>
        void SubmitResource(const Ref<T> &resource)
        {
            m_Resources.push_back(std::static_pointer_cast<void>(resource));
        }

        void ResetResources()
        {
            m_Resources.clear();
        }

    private:
        Unique<Semaphore> m_ImageAvailableSemaphore;
        Unique<Semaphore> m_RenderFinishedSemaphore;
        Unique<Fence> m_InFlightFence;
        Unique<CommandBuffer> m_CommandBuffer;

        vector<Ref<void>> m_Resources{};
    };
}
