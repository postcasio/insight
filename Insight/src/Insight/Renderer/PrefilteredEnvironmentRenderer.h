#pragma once

#include "CommandBuffer.h"
#include "Vulkan.h"

namespace Insight
{
    class StaticMesh;
}

namespace Insight::Renderer
{
    struct RenderPrefilteredEnvironmentInfo
    {
        Image& Cubemap;
        Image& Target;
    };

    struct PrefilteredEnvironmentPushConstants
    {
        alignas(16) mat4 Projection;
        alignas(16) mat4 View;
        alignas(4) f32 Roughness = 0;
    };

    class PrefilteredEnvironmentRenderer
    {
    public:
        explicit PrefilteredEnvironmentRenderer(const vk::Format format);

        void Render(const RenderPrefilteredEnvironmentInfo& info);

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
