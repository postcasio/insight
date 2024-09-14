#pragma once

#include "CommandBuffer.h"
#include "Vulkan.h"

namespace Insight
{
    class StaticMesh;
}

namespace Insight::Renderer
{
    struct RenderEquirectangularToCubemapInfo
    {
        Image& EquirectangularMap;
        Image& Target;
    };

    struct EquirectangularToCubemapPushConstants
    {
        alignas(16) mat4 Projection;
        alignas(16) mat4 View;
    };

    class EquirectangularToCubemapRenderer
    {
    public:
        explicit EquirectangularToCubemapRenderer(const vk::Format format);

        void Render(const RenderEquirectangularToCubemapInfo& info);

    private:
        vk::Format m_OutputFormat;
        Unique<DescriptorSetLayout> m_DescriptorSetLayout;
        Ref<DescriptorSet> m_DescriptorSet;
        Ref<Sampler> m_Sampler;
        Unique<PipelineLayout> m_PipelineLayout;
        Unique<DynamicGraphicsPipeline> m_Pipeline;
        Ref<StaticMesh> m_CubeMesh;
    };
}
