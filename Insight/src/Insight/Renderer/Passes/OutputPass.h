#pragma once

#include "../../Insight.h"
#include "../../Math.h"
#include "../CommandBuffer.h"

namespace Insight
{
    class Scene;
}

namespace Insight::Renderer
{
    class CommandBuffer;
    class DescriptorSet;
    class DescriptorSetLayout;
    class Sampler;
    class ImageView;
    class Image;

    struct OutputPassExecuteInfo
    {
        CommandBuffer& CommandBuffer;
        DescriptorSet& LightingSamplerDescriptorSet;
        DescriptorSet& BloomSamplerDescriptorSet;
        Image& Target;
        ImageView& TargetView;
    };

    struct OutputPassInfo
    {
        vk::Format Format;
        DescriptorSetLayout& LightingSamplerDescriptorSetLayout;
        DescriptorSetLayout& BloomSamplerDescriptorSetLayout;
    };

    class OutputPass
    {
    public:
        explicit OutputPass(const OutputPassInfo& info);
        void CreateResources();

        void SetExtent(uvec2 extent)
        {
            m_Extent = extent;
        }

        void Execute(const OutputPassExecuteInfo& info);

    private:
        [[nodiscard]] vector<RenderingAttachmentInfo> GetAttachments() const;

        uvec2 m_Extent{};
        vk::Format m_Format;

        Ref<Sampler> m_Sampler;

        Unique<PipelineLayout> m_PipelineLayout;
        Unique<DynamicGraphicsPipeline> m_Pipeline;
    };
}
