#pragma once

#include "PipelineLayout.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Vulkan.h"

namespace Insight::Renderer
{
    struct DynamicPipelineInfo
    {
        string Name;

        vector<PipelineAttachmentInfo> ColorAttachments = {};
        vk::Format DepthAttachmentFormat = vk::Format::eUndefined;
        vk::Format StencilAttachmentFormat = vk::Format::eUndefined;

        optional<VertexBufferLayout> VertexBufferLayout;
        PipelineLayout& PipelineLayout;

        vector<PipelineShaderStageInfo> ShaderStages = {};
        vk::PolygonMode PolygonMode = vk::PolygonMode::eFill;
        vk::CullModeFlags CullMode = vk::CullModeFlagBits::eNone;
        bool DepthTestEnable = false;
        bool DepthWriteEnable = false;
        vk::CompareOp DepthCompareOp = vk::CompareOp::eLessOrEqual;
    };

    class DynamicGraphicsPipeline
    {
    public:
        static Unique<DynamicGraphicsPipeline> Create(const DynamicPipelineInfo& info)
        {
            return CreateUnique<DynamicGraphicsPipeline>(info);
        }

        explicit DynamicGraphicsPipeline(const DynamicPipelineInfo& info);
        ~DynamicGraphicsPipeline();

        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::Pipeline GetVkPipeline() const { return m_VkPipeline; }

    private:
        string m_Name;
        vk::Pipeline m_VkPipeline;
    };
}
