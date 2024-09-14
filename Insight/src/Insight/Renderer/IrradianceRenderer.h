#pragma once

#include "CommandBuffer.h"
#include "Vulkan.h"

namespace Insight
{
    class StaticMesh;
}

namespace Insight::Renderer
{
    struct RenderIrradianceInfo
    {
        Image& Cubemap;
        Image& Target;
    };

    struct IrradiancePushConstants
    {
        alignas(16) mat4 Projection;
        alignas(16) mat4 View;
    };

    class IrradianceRenderer
    {
    public:
        explicit IrradianceRenderer(const vk::Format format);

        void Render(const RenderIrradianceInfo& info);

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
