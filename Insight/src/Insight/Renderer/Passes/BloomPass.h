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

    struct BloomPassExecuteInfo
    {
        CommandBuffer& CommandBuffer;
        DescriptorSet& InputSamplerDescriptorSet;
    };

    struct BloomPassInfo
    {
        DescriptorSetLayout& InputSamplerDescriptorSetLayout;
    };

    class BloomPass
    {
    public:
        BloomPass(const BloomPassInfo& info);
        void CreateResources();

        void SetExtent(uvec2 extent)
        {
            m_Extent = extent;
        }

        void Execute(const BloomPassExecuteInfo& info);
        [[nodiscard]] DescriptorSetLayout& GetSamplerDescriptorSetLayout() const { return *m_SamplerDescriptorSetLayout; }
        [[nodiscard]] Ref<DescriptorSet> GetSamplerDescriptorSet() { return m_SamplerDescriptorSets[0]; }
        [[nodiscard]] Ref<Image> GetOutputImage() { return m_MipImages[0]; }
        [[nodiscard]] Ref<ImageView> GetOutputView() { return m_MipViews[0]; }

    private:
        uvec2 m_Extent{};
        uint m_MipCount;

        Ref<Sampler> m_Sampler;

        Unique<DescriptorSetLayout> m_SamplerDescriptorSetLayout;
        vector<Ref<DescriptorSet>> m_SamplerDescriptorSets;

        Unique<PipelineLayout> m_PipelineLayout;
        Unique<DynamicGraphicsPipeline> m_DownPipeline;
        Unique<DynamicGraphicsPipeline> m_UpPipeline;

        vector<Ref<Image>> m_MipImages;
        vector<Ref<ImageView>> m_MipViews;
    };
}
