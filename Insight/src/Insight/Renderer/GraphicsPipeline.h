#pragma once

#include "PipelineLayout.h"
#include "RenderPass.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Vulkan.h"

namespace Insight::Renderer
{
    struct GraphicsPipelineInfo
    {
        string Name;

        optional<VertexBufferLayout> VertexBufferLayout;
        PipelineLayout& PipelineLayout;

        vector<PipelineShaderStageInfo> ShaderStages = {};
        vk::PolygonMode PolygonMode = vk::PolygonMode::eFill;
        vk::CullModeFlags CullMode = vk::CullModeFlagBits::eBack;
        bool DepthTestEnable = true;
        bool DepthWriteEnable = true;
        vk::CompareOp DepthCompareOp = vk::CompareOp::eLessOrEqual;
        vector<vk::PipelineColorBlendAttachmentState> ColorBlendAttachments = {};

        RenderPass &RenderPass;
    };

    class GraphicsPipeline
    {
    public:
        static Unique<GraphicsPipeline> Create(const GraphicsPipelineInfo& info)
        {
            return CreateUnique<GraphicsPipeline>(info);
        }

        explicit GraphicsPipeline(const GraphicsPipelineInfo& info);
        ~GraphicsPipeline();

        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::Pipeline GetVkPipeline() const { return m_VkPipeline; }

    private:
        string m_Name;
        vk::Pipeline m_VkPipeline;
    };
}
