#include "LightingPass.h"

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
    LightingPass::LightingPass(const LightingPassInfo& info)
    {
        m_SamplerDescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "Lighting Samplers",
            .Bindings = {
                {
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .binding = 0,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment
                }
            }
        });

        m_Sampler = Sampler::Create({
            .Name = "Lighting Sampler",
            .MagFilter = vk::Filter::eNearest,
            .MinFilter = vk::Filter::eNearest,
            .MipmapMode = vk::SamplerMipmapMode::eNearest,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        m_PipelineLayout = PipelineLayout::Create({
            .Name = "Lighting",
            .DescriptorSetLayouts = {
                {Context::Instance().GetSceneDescriptorSetLayout()},
                {info.GBufferSamplerDescriptorSetLayout}
            }
        });

        m_Pipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "SceneRenderer Lighting",
            .ColorAttachments = {
                {
                    // Output
                    .Format = Context::Instance().GetOutputFormat()
                },
            },
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Lighting (Vertex)",
                        .Path = "Engine/Shaders/FullscreenTriangle.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "Lighting (Fragment)",
                        .Path = "Engine/Shaders/Lighting.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eAlways,
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .VertexBufferLayout = {}
        });

    }

    void LightingPass::CreateResources()
    {
        m_LightingImage = Image::Create({
            .Name = "Lighting",
            .Extent = {m_Extent, 1},
            .Format = Context::Instance().GetOutputFormat(),
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_LightingView = ImageView::Create({
            .Name = "Albedo",
            .Image = *m_LightingImage
        });

        m_SamplerDescriptorSet = DescriptorSet::Create({
            .Name = "Lighting Samplers",
            .Layout = *m_SamplerDescriptorSetLayout
        });

        m_SamplerDescriptorSet->UpdateDescriptorSet({
            .Writes = {
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 0,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_LightingView,
                            .Sampler = *m_Sampler
                        }
                    }
                }
            }
        });
    }

    void LightingPass::Execute(const LightingPassExecuteInfo& info)
    {
        Command::SubmitResource(m_LightingImage);
        Command::SubmitResource(m_LightingView);
        Command::SubmitResource(m_SamplerDescriptorSet);

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_LightingImage,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.BeginRendering({
            .Extent = m_Extent,
            .Offset = {0, 0},
            .ColorAttachments = {
                {
                    .ImageView = *m_LightingView,
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
                {.Set = info.SceneDataDescriptorSet},
                {.Set = info.GBufferSamplerDescriptorSet},
            },
            .PipelineLayout = *m_PipelineLayout
        });

        info.CommandBuffer.DrawFullscreenTriangle();

        info.CommandBuffer.EndRendering();

        const Entity environmentEntity = {info.Scene.GetRegistry().view<EnvironmentComponent>().front(), &info.Scene};

        if (environmentEntity.IsValid())
        {
            const auto& environment = environmentEntity.GetComponent<EnvironmentComponent>().Environment;

            environment->Render({
                .CommandBuffer = info.CommandBuffer,
                .Target = *m_LightingImage,
                .TargetView = *m_LightingView,
                .DepthTarget = info.DepthTarget,
                .DepthTargetView = info.DepthTargetView,
                .SceneDataDescriptorSet = info.SceneDataDescriptorSet,
            });
        }

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_LightingImage,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });
    }
}
