#include "PipelineLayout.h"

#include "Context.h"
#include "DebugMarkers.h"

namespace Insight::Renderer {
    PipelineLayout::PipelineLayout(const PipelineLayoutInfo& info) : m_Name(info.Name)
    {
        vector<vk::DescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(info.DescriptorSetLayouts.size());

        for (const auto& descriptorSetLayoutInfo : info.DescriptorSetLayouts)
        {
            descriptorSetLayouts.push_back(descriptorSetLayoutInfo.Layout.GetVkDescriptorSetLayout());
        }

        vector<vk::PushConstantRange> pushConstantRanges;
        pushConstantRanges.reserve(info.PushConstantRanges.size());

        for (const auto& pushConstantRangeInfo : info.PushConstantRanges)
        {
            pushConstantRanges.push_back({
                .stageFlags = pushConstantRangeInfo.Stages,
                .offset = pushConstantRangeInfo.Offset,
                .size = pushConstantRangeInfo.Size
            });
        }

        const vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo {
            .flags = info.Flags,
            .setLayoutCount = static_cast<u32>(descriptorSetLayouts.size()),
            .pSetLayouts = descriptorSetLayouts.data(),
            .pushConstantRangeCount = static_cast<u32>(pushConstantRanges.size()),
            .pPushConstantRanges = pushConstantRanges.data(),
        };

        m_VkPipelineLayout = Context::Instance().GetVkDevice().createPipelineLayout(pipelineLayoutCreateInfo);

        DebugMarkers::MarkPipelineLayout(m_VkPipelineLayout, m_Name);
    }

    PipelineLayout::~PipelineLayout()
    {
        Context::Instance().GetVkDevice().destroyPipelineLayout(m_VkPipelineLayout);
    }

}
