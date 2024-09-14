#pragma once

#include "DescriptorSetLayout.h"
#include "Shader.h"

namespace Insight::Renderer
{
    struct PipelineShaderStageInfo
    {
        vk::ShaderStageFlagBits Stage;
        Shader& Module;
    };

    struct PipelineAttachmentInfo
    {
        vk::Format Format = vk::Format::eUndefined;
        vk::PipelineColorBlendAttachmentState BlendMode = {
            .blendEnable = VK_FALSE,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
        };
    };

    struct PipelineLayoutDescriptorSetLayoutInfo
    {
        DescriptorSetLayout &Layout;
    };

    struct PipelineLayoutPushConstantRangeInfo
    {
        vk::ShaderStageFlags Stages {};
        u32 Offset = 0;
        u32 Size;
    };

    struct PipelineLayoutInfo
    {
        string Name;
        vk::PipelineLayoutCreateFlags Flags {};
        vector<PipelineLayoutDescriptorSetLayoutInfo> DescriptorSetLayouts {};
        vector<PipelineLayoutPushConstantRangeInfo> PushConstantRanges {};
    };

    class PipelineLayout
    {
    public:
        static Unique<PipelineLayout> Create(const PipelineLayoutInfo& info)
        {
            return CreateUnique<PipelineLayout>(info);
        }

        explicit PipelineLayout(const PipelineLayoutInfo& info);
        ~PipelineLayout();

        [[nodiscard]] vk::PipelineLayout GetVkPipelineLayout() const { return m_VkPipelineLayout; }
        [[nodiscard]] string GetName() const { return m_Name; }

    private:
        string m_Name;
        vk::PipelineLayout m_VkPipelineLayout = nullptr;
    };
}
