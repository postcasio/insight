#include "OutputPass.h"

#include "../Image.h"
#include "../Context.h"
#include "../../Assets/Material.h"
#include "../../Scene.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/StaticMeshComponent.h"
#include "../../Components/EnvironmentComponent.h"
#include "../Command.h"
#include "../../Entity.h"

namespace Insight::Renderer
{
    OutputPass::OutputPass(const OutputPassInfo& info) : m_Format(info.Format)
    {
        m_Sampler = Sampler::Create({
            .Name = "Output Sampler",
            .MagFilter = vk::Filter::eNearest,
            .MinFilter = vk::Filter::eNearest,
            .MipmapMode = vk::SamplerMipmapMode::eNearest,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        m_PipelineLayout = PipelineLayout::Create({
            .Name = "Output",
            .DescriptorSetLayouts = {
                {info.LightingSamplerDescriptorSetLayout},
                {info.BloomSamplerDescriptorSetLayout},
            }
        });

        m_Pipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "SceneRenderer Output",
            .ColorAttachments = {
                {
                    // Output
                    .Format = info.Format
                },
            },
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Output (Vertex)",
                        .Path = "Engine/Shaders/FullscreenTriangle.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "Output (Fragment)",
                        .Path = "Engine/Shaders/Output.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eAlways,
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .VertexBufferLayout = {}
        });

    }

    void OutputPass::CreateResources()
    {

    }

    void OutputPass::Execute(const OutputPassExecuteInfo& info)
    {
        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.BeginRendering({
            .Extent = m_Extent,
            .Offset = {0, 0},
            .ColorAttachments = {
                {
                    .ImageView = info.TargetView,
                    .LoadOp = vk::AttachmentLoadOp::eClear,
                    .StoreOp = vk::AttachmentStoreOp::eStore,
                    .ClearValue = {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}}
                }
            },
            .LayerCount = 1,
        });

        info.CommandBuffer.SetScissor({0, 0}, m_Extent);
        info.CommandBuffer.SetViewport({0, 0}, m_Extent);

        info.CommandBuffer.BindPipeline(*m_Pipeline);
        info.CommandBuffer.BindDescriptorSets({
            .Sets = {
                {.Set = info.LightingSamplerDescriptorSet},
                {.Set = info.BloomSamplerDescriptorSet},
            },
            .PipelineLayout = *m_PipelineLayout
        });

        info.CommandBuffer.Draw(3, 1, 0, 0);

        info.CommandBuffer.EndRendering();

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });
    }
}
