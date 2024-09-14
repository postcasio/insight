#include "GraphicsPipeline.h"

#include "Context.h"
#include "DebugMarkers.h"
#include "Utils.h"

namespace Insight::Renderer
{

    GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineInfo& info) : m_Name(info.Name)
    {
        vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        shaderStages.reserve(info.ShaderStages.size());

        for (const auto& shaderStage : info.ShaderStages)
        {
            shaderStages.push_back({
                .stage = shaderStage.Stage,
                .module = shaderStage.Module.GetVkModule(),
                .pName = shaderStage.Module.GetEntryPoint().c_str()
            });
        }

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
        vk::VertexInputBindingDescription bindingDescription;
        vector<vk::VertexInputAttributeDescription> attributeDescriptions;

        if (info.VertexBufferLayout.has_value())
        {
            auto& vertexBufferLayout = info.VertexBufferLayout.value();

            bindingDescription = {
                bindingDescription.binding = 0,
                bindingDescription.stride = vertexBufferLayout.GetStride(),
                bindingDescription.inputRate = vk::VertexInputRate::eVertex,
            };

            auto& elements = vertexBufferLayout.GetElements();

            for (u32 i = 0; i < elements.size(); i++)
            {
                auto& element = elements[i];

                vk::VertexInputAttributeDescription attributeDescription = {
                    .binding = 0,
                    .location = i,
                    .format = VertexElementDataTypeToVulkanFormat(element.Type),
                    .offset = element.Offset,
                };

                attributeDescriptions.push_back(attributeDescription);
            }

            vertexInputInfo = {
                .vertexBindingDescriptionCount = 1,
                .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
                .pVertexBindingDescriptions = &bindingDescription,
                .pVertexAttributeDescriptions = attributeDescriptions.data(),
            };
        }
        else
        {
            vertexInputInfo = {
                .vertexAttributeDescriptionCount = 0,
                .pVertexAttributeDescriptions = nullptr,
                .vertexBindingDescriptionCount = 0,
                .pVertexBindingDescriptions = nullptr
            };
        }

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
            .topology = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = VK_FALSE
        };

        vk::PipelineTessellationStateCreateInfo tessellationState{
            .patchControlPoints = 0
        };

        vk::PipelineViewportStateCreateInfo viewportState{
            .viewportCount = 1,
            .scissorCount = 1
        };

        vk::PipelineRasterizationStateCreateInfo rasterizerState = {
            .depthClampEnable = vk::False,
            .rasterizerDiscardEnable = vk::False,
            .polygonMode = info.PolygonMode,
            .lineWidth = 1.0f,
            .cullMode = info.CullMode,
            .frontFace = vk::FrontFace::eCounterClockwise,
            .depthBiasEnable = vk::False,
        };

        vk::PipelineMultisampleStateCreateInfo multisampleState = {
            .sampleShadingEnable = vk::False,
            .rasterizationSamples = vk::SampleCountFlagBits::e1
        };

        vk::PipelineDepthStencilStateCreateInfo depthStencilState = {
            .depthTestEnable = VK_BOOL(info.DepthTestEnable),
            .depthWriteEnable = VK_BOOL(info.DepthWriteEnable),
            .depthCompareOp = info.DepthCompareOp,
            .depthBoundsTestEnable = vk::False,
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
            .stencilTestEnable = vk::False,
            .front = {},
            .back = {}
        };

        std::vector<vk::PipelineColorBlendAttachmentState> blendAttachments;
        blendAttachments.reserve(info.ColorBlendAttachments.size());

        for (auto &attachment : info.ColorBlendAttachments)
        {
            blendAttachments.push_back(attachment);
        }

        vk::PipelineColorBlendStateCreateInfo colorBlendState = {
            .logicOpEnable = vk::False,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = static_cast<u32>(blendAttachments.size()),
            .pAttachments = blendAttachments.data(),
            .blendConstants = {{0, 0, 0, 0,}},
        };

        vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState = {
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates = dynamicStates.data()
        };

        const vk::GraphicsPipelineCreateInfo pipelineInfo = {
            .stageCount = static_cast<u32>(shaderStages.size()),
            .pStages = shaderStages.data(),
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pTessellationState = &tessellationState,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizerState,
            .pMultisampleState = &multisampleState,
            .pDepthStencilState = &depthStencilState,
            .pColorBlendState = &colorBlendState,
            .pDynamicState = &dynamicState,
            .layout = info.PipelineLayout.GetVkPipelineLayout(),
            .renderPass = info.RenderPass.GetVkRenderPass(),
            .subpass = 0,
            .basePipelineHandle = nullptr,
            .basePipelineIndex = 0,
        };

        m_VkPipeline = Context::Instance().GetVkDevice().createGraphicsPipeline(nullptr, pipelineInfo).value;

        DebugMarkers::MarkPipeline(m_VkPipeline, m_Name);
    }

    GraphicsPipeline::~GraphicsPipeline()
    {
        Context::Instance().GetVkDevice().destroyPipeline(m_VkPipeline);
    }
}
