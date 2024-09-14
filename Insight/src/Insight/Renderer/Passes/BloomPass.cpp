#include "BloomPass.h"

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
    BloomPass::BloomPass(const BloomPassInfo& info)
    {
        m_SamplerDescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "Bloom Samplers",
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
            .Name = "Bloom Sampler",
            .MagFilter = vk::Filter::eLinear,
            .MinFilter = vk::Filter::eLinear,
            .MipmapMode = vk::SamplerMipmapMode::eLinear,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        m_PipelineLayout = PipelineLayout::Create({
            .Name = "Bloom",
            .DescriptorSetLayouts = {
                {info.InputSamplerDescriptorSetLayout},
            }
        });

        m_DownPipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "SceneRenderer Bloom Down",
            .ColorAttachments = {
                {
                    .Format = Context::Instance().GetOutputFormat()
                },
            },
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Bloom (Vertex)",
                        .Path = "Engine/Shaders/FullscreenTriangle.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "Bloom (Fragment)",
                        .Path = "Engine/Shaders/BloomDown.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eAlways,
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .VertexBufferLayout = {}
        });

        m_UpPipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "SceneRenderer Bloom Up",
            .ColorAttachments = {
                {
                    .Format = Context::Instance().GetOutputFormat(),
                    .BlendMode = {
                        .blendEnable = vk::True,
                        .srcColorBlendFactor = vk::BlendFactor::eOne,
                        .dstColorBlendFactor = vk::BlendFactor::eOne,
                        .colorBlendOp = vk::BlendOp::eAdd,
                        .srcAlphaBlendFactor = vk::BlendFactor::eOne,
                        .dstAlphaBlendFactor = vk::BlendFactor::eOne,
                        .alphaBlendOp = vk::BlendOp::eAdd,
                        .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA,
                    }
                },
            },
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Bloom (Vertex)",
                        .Path = "Engine/Shaders/FullscreenTriangle.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "Bloom (Fragment)",
                        .Path = "Engine/Shaders/BloomUp.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eAlways,
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .VertexBufferLayout = {}
        });
    }

    void BloomPass::CreateResources()
    {
        const auto mips = std::min(7u, CalculateMipLevels(m_Extent.x, m_Extent.y));

        for (auto i = 0; i < mips; i++)
        {
            const auto image = Image::Create({
                .Name = fmt::format("Bloom Mip {0}", i),
                .Extent = {std::max(m_Extent.x >> i, 1u), std::max(m_Extent.y >> i, 1u), 1},
                .MipLevels = 1,
                .Format = Context::Instance().GetOutputFormat(),
                .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
                .Filter = vk::Filter::eLinear,
                .Type = vk::ImageType::e2D
            });

            const auto view = ImageView::Create({
                .Name = fmt::format("Bloom Mip {0}", i),
                .Image = *image
            });

            m_MipImages.push_back(image);
            m_MipViews.push_back(view);
        }

        m_MipCount = m_MipImages.size();

        m_SamplerDescriptorSets.clear();

        for (auto i = 0; i < m_MipCount; i++)
        {
            const auto set = DescriptorSet::Create({
                .Name = fmt::format("Bloom Sampler {0}", i),
                .Layout = *m_SamplerDescriptorSetLayout
            });

            set->UpdateDescriptorSet({
                .Writes = {
                    {
                        .Type = vk::DescriptorType::eCombinedImageSampler,
                        .Binding = 0,
                        .Data = vector<DescriptorImageInfo>{
                            {
                                .ImageView = *m_MipViews[i],
                                .Sampler = *m_Sampler
                            }
                        }
                    }
                }
            });

            m_SamplerDescriptorSets.push_back(set);
        }
    }

    void BloomPass::Execute(const BloomPassExecuteInfo& info)
    {
        // Downsample

        for (auto i = 0; i < m_MipCount; i++)
        {
            info.CommandBuffer.PipelineBarrier({
                .Image = *m_MipImages[i],
                .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
            });


            info.CommandBuffer.SetScissor({0, 0}, m_MipImages[i]->GetExtent());
            info.CommandBuffer.SetViewport({0, 0}, m_MipImages[i]->GetExtent());

            info.CommandBuffer.BeginRendering({
                .Extent = m_MipImages[i]->GetExtent(),
                .Offset = {0, 0},
                .ColorAttachments = {
                    {
                        .ImageView = *m_MipViews[i],
                        .LoadOp = vk::AttachmentLoadOp::eClear,
                        .StoreOp = vk::AttachmentStoreOp::eStore,
                        .ClearValue = {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}}
                    }
                },
            });

            info.CommandBuffer.BindPipeline(*m_DownPipeline);

            if (i == 0)
            {
                info.CommandBuffer.BindDescriptorSets({
                    .Sets = {
                        {info.InputSamplerDescriptorSet}
                    },
                    .PipelineLayout = *m_PipelineLayout
                });
            }

            else
            {
                info.CommandBuffer.BindDescriptorSets({
                    .Sets = {
                        {*m_SamplerDescriptorSets[i - 1]}
                    },
                    .PipelineLayout = *m_PipelineLayout
                });
            }

            info.CommandBuffer.Draw(3, 1, 0, 0);

            info.CommandBuffer.EndRendering();

            info.CommandBuffer.PipelineBarrier(ImageBarrier{
                .Image = *m_MipImages[i],
                .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
            });
        }

        // Upsample

        for (auto i = m_MipCount - 1; i > 0; i--)
        {
            info.CommandBuffer.PipelineBarrier({
                .Image = *m_MipImages[i - 1],
                .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
            });

            info.CommandBuffer.SetScissor({0, 0}, m_MipImages[i - 1]->GetExtent());
            info.CommandBuffer.SetViewport({0, 0}, m_MipImages[i - 1]->GetExtent());

            info.CommandBuffer.BeginRendering({
                .Extent = m_MipImages[i - 1]->GetExtent(),
                .Offset = {0, 0},
                .ColorAttachments = {
                    {
                        .ImageView = *m_MipViews[i - 1],
                        .LoadOp = vk::AttachmentLoadOp::eLoad,
                        .StoreOp = vk::AttachmentStoreOp::eStore,
                        .ClearValue = {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}}
                    }
                },
            });

            info.CommandBuffer.BindPipeline(*m_UpPipeline);
            info.CommandBuffer.BindDescriptorSets({
                .Sets = {
                    {*m_SamplerDescriptorSets[i]}
                },
                .PipelineLayout = *m_PipelineLayout
            });

            info.CommandBuffer.DrawFullscreenTriangle();

            info.CommandBuffer.EndRendering();

            info.CommandBuffer.PipelineBarrier(ImageBarrier{
                .Image = *m_MipImages[i - 1],
                .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
            });
        }
    }
}
