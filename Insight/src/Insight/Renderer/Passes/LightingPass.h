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

    struct LightingPassExecuteInfo
    {
        CommandBuffer& CommandBuffer;
        Scene& Scene;
        Image& DepthTarget;
        ImageView& DepthTargetView;
        DescriptorSet& SceneDataDescriptorSet;
        DescriptorSet& GBufferSamplerDescriptorSet;
    };

    struct LightingPassInfo
    {
        DescriptorSetLayout& GBufferSamplerDescriptorSetLayout;
    };

    class LightingPass
    {
    public:
        LightingPass(const LightingPassInfo& info);
        void CreateResources();

        void SetExtent(uvec2 extent)
        {
            m_Extent = extent;
        }

        void Execute(const LightingPassExecuteInfo& info);
        [[nodiscard]] DescriptorSetLayout& GetSamplerDescriptorSetLayout() const { return *m_SamplerDescriptorSetLayout; }
        [[nodiscard]] Ref<DescriptorSet> GetSamplerDescriptorSet() { return m_SamplerDescriptorSet; }
        [[nodiscard]] Ref<Image> GetLightingImage() { return m_LightingImage; }
        [[nodiscard]] Ref<ImageView> GetLightingView() { return m_LightingView; }

    private:
        uvec2 m_Extent{};

        Ref<Image> m_LightingImage;
        Ref<ImageView> m_LightingView;

        Ref<Sampler> m_Sampler;

        Unique<DescriptorSetLayout> m_SamplerDescriptorSetLayout;
        Ref<DescriptorSet> m_SamplerDescriptorSet;

        Unique<PipelineLayout> m_PipelineLayout;
        Unique<DynamicGraphicsPipeline> m_Pipeline;

    };
}
