#include "PrefilteredEnvironmentRenderer.h"

#include "Command.h"
#include "Context.h"
#include "../Geometry/BoxGeometry.h"

namespace Insight::Renderer
{
    PrefilteredEnvironmentRenderer::PrefilteredEnvironmentRenderer(const vk::Format format) : m_OutputFormat(format)
    {
        BoxGeometry cube({.Width = 1, .Height = 1, .Depth = 1});
        
        m_CubeMesh = StaticMesh::Create({
            .Name = "Cube",
            .Indices = cube.GetIndices(),
            .Vertices = cube.GetVertices(),
            .Submeshes = cube.GetSubmeshes()
        });

        m_DescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "PrefilteredEnvironment Samplers",
            .Bindings = {
                {
                    .binding = 0,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex
                }
            }
        });

        m_DescriptorSet = DescriptorSet::Create({
            .Name = "Scene Data",
            .Layout = *m_DescriptorSetLayout
        });

        m_Sampler = Sampler::Create({
            .Name = "PrefilteredEnvironment Sampler",
            .MagFilter = vk::Filter::eLinear,
            .MinFilter = vk::Filter::eLinear,
            .MipmapMode = vk::SamplerMipmapMode::eLinear,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        m_PipelineLayout = PipelineLayout::Create({
            .Name = "PrefilteredEnvironment",
            .DescriptorSetLayouts = {
                {*m_DescriptorSetLayout}
            },
            .PushConstantRanges = {
                {
                    .Stages = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                    .Offset = 0,
                    .Size = sizeof(PrefilteredEnvironmentPushConstants)
                }
            }
        });

        m_Pipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "PrefilteredEnvironment",
            .ColorAttachments = {
                {
                    // Output
                    .Format = m_OutputFormat
                },
            },
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "PrefilteredEnvironment (Vertex)",
                        .Path = "Engine/Shaders/EnvironmentPrefilter.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "PrefilteredEnvironment (Fragment)",
                        .Path = "Engine/Shaders/EnvironmentPrefilter.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eAlways,
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .VertexBufferLayout = Vertex3D::GetLayout()
        });
    }

    void PrefilteredEnvironmentRenderer::Render(const RenderPrefilteredEnvironmentInfo& info)
    {
        auto commandBuffer = Renderer::CommandBuffer::Create();

        commandBuffer->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        const auto cubemapView = ImageView::Create({
            .Image = info.Cubemap,
            .ViewType = vk::ImageViewType::eCube,
            .ArrayLayers = 6
        });

        const auto mipLevels = info.Target.GetMipLevels();

        vector<vector<Ref<ImageView>>> cubeFaceViews;

        for (u32 mip = 0; mip < mipLevels; mip++)
        {
            vector<Ref<ImageView>> mipFaceViews;

            for (u32 i = 0; i < 6; i++)
            {
                const string faceName = info.Target.GetName() + " Face " + std::to_string(i);

                mipFaceViews.push_back(ImageView::Create({
                    .Name = faceName,
                    .Image = info.Target,
                    .ArrayLayers = 1,
                    .BaseArrayLayer = i,
                    .BaseMipLevel = mip,
                    .MipLevels = 1
                }));
            }

            cubeFaceViews.push_back(mipFaceViews);
        }

        m_DescriptorSet->UpdateDescriptorSet({
            .Writes = {
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 0,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *cubemapView,
                            .Sampler = *m_Sampler
                        }
                    }
                }
            }
        });

        commandBuffer->PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal,
            .LayerCount = info.Target.GetLayers(),
            .MipLevelCount = info.Target.GetMipLevels()
        });

        uvec2 extent = {info.Target.GetWidth(), info.Target.GetHeight()};

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        // captureProjection[1][1] *= -1;

        glm::mat4 captureViews[] =
        {
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))
        };


        PrefilteredEnvironmentPushConstants parametersPushConstants = {
            .Projection = captureProjection
        };

        // RENDER MIP LEVELS
        for (u32 mip = 0; mip < info.Target.GetMipLevels(); mip++)
        {
            u32 mipWidth = info.Target.GetWidth() * std::pow(0.5, mip);
            u32 mipHeight = info.Target.GetHeight() * std::pow(0.5, mip);

            extent = {std::max(1u, mipWidth), std::max(1u, mipHeight)};

            parametersPushConstants.Roughness = static_cast<f32>(mip) / static_cast<f32>(info.Target.GetMipLevels() - 1);

            // RENDER CUBEMAP FACES
            for (u32 i = 0; i < 6; i++)
            {
                commandBuffer->BeginRendering({
                    .Extent = {mipWidth, mipHeight},
                    .Offset = {0, 0},
                    .ColorAttachments = {
                        {
                            .ImageView = *cubeFaceViews[mip][i],
                            .LoadOp = vk::AttachmentLoadOp::eClear,
                            .StoreOp = vk::AttachmentStoreOp::eStore,
                            .ClearValue = vk::ClearValue{
                                .color = std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})
                            }
                        },
                    },
                    .DepthAttachment = {},
                    .LayerCount = 1,
                });

                commandBuffer->SetScissor({0, 0}, extent);
                commandBuffer->SetViewport({0, 0}, extent);

                commandBuffer->BindPipeline(*m_Pipeline);

                commandBuffer->BindDescriptorSets({
                    .Sets = {{*m_DescriptorSet}},
                    .PipelineLayout = *m_PipelineLayout
                });

                parametersPushConstants.View = captureViews[i];

                commandBuffer->PushConstants({
                    .Data = &parametersPushConstants,
                    .Offset = 0,
                    .Size = sizeof(PrefilteredEnvironmentPushConstants),
                    .PipelineLayout = *m_PipelineLayout,
                    .StageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
                });

                m_CubeMesh->BindBuffers(*commandBuffer);
                m_CubeMesh->DrawAllSubmeshes(*commandBuffer);

                commandBuffer->EndRendering();
            }
        }

        commandBuffer->PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
            .BaseLayer = 0,
            .LayerCount = info.Target.GetLayers(),
            .BaseMipLevel = 0,
            .MipLevelCount = info.Target.GetMipLevels()
        });

        commandBuffer->End();

        Context::Instance().SubmitImmediateCommands(*commandBuffer);
    }
}
