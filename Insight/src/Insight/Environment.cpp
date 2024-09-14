#include "Environment.h"

#include "ImageUtils.h"
#include "Geometry/BoxGeometry.h"
#include "Renderer/Command.h"
#include "Renderer/Context.h"
#include "Renderer/SceneRenderer.h"

namespace Insight
{
    Environment::Environment()
    {
        m_EnvironmentBuffer = Renderer::Buffer::Create({
            .Name = "Environment Buffer",
            .Size = sizeof(EnvironmentUniform),
            .Usage = vk::BufferUsageFlagBits::eUniformBuffer
        });

        m_EmptyEnvironmentMap = CreateTexture({
            .Name = "Empty Environment Map",
            .Width = 512,
            .Height = 256,
            .ClearColor = vec4(0, 0, 0, 1)
        });

        UpdateEnvironmentCubemap();

        BoxGeometry cubeGeometry({1.0f, 1.0f, 1.0f});

        m_SkyboxMesh = StaticMesh::Create({
            .Name = "Skybox",
            .Vertices = cubeGeometry.GetVertices(),
            .Indices = cubeGeometry.GetIndices(),
            .Submeshes = cubeGeometry.GetSubmeshes()
        });

        m_PipelineLayout = Renderer::PipelineLayout::Create({
            .Name = "Environment",
            .DescriptorSetLayouts = {
                {Renderer::Context::Instance().GetSceneDescriptorSetLayout()},
                {Renderer::Context::Instance().GetEnvironmentDescriptorSetLayout()}
            }
        });

        m_Pipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "Equirectangular to Cubemap",
            .ColorAttachments = {
                {
                    // Output
                    .Format = Renderer::Context::Instance().GetOutputFormat(),
                },
            },
            .DepthAttachmentFormat = Renderer::Context::Instance().GetDepthFormat(),
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Lighting (Vertex)",
                        .Path = "Engine/Shaders/Environment.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment, .Module = *Renderer::Shader::Create({
                        .Name = "Lighting (Fragment)",
                        .Path = "Engine/Shaders/Environment.frag.spv",
                    })
                },
            },
            .DepthCompareOp = vk::CompareOp::eLessOrEqual,
            .DepthTestEnable = true,
            .DepthWriteEnable = false,
            .VertexBufferLayout = Renderer::Vertex3D::GetLayout()
        });

        UpdateDescriptorSet();
    }

    void Environment::UpdateEnvironmentCubemap()
    {
        if (m_EnvironmentMap)
        {
            m_EnvironmentCubemap = m_EnvironmentMap->EquirectangularToCubemap();
        }
        else
        {
            m_EnvironmentCubemap = m_EmptyEnvironmentMap->EquirectangularToCubemap();
        }

        UpdateEnvironmentIrradianceMap();
    }

    void Environment::SetEnvironmentMap(const Ref<Texture>& environmentMap)
    {
        m_EnvironmentMap = environmentMap;

        UpdateEnvironmentCubemap();

        m_EnvironmentUniform.HasEnvironmentMap = 1;

        UpdateDescriptorSet();
    }

    void Environment::UpdateEnvironmentIrradianceMap()
    {
        if (m_EnvironmentCubemap)
        {
            m_EnvironmentIrradianceMap = m_EnvironmentCubemap->CreateIrradianceMap();
            m_PrefilteredEnvironmentCubemap = m_EnvironmentCubemap->CreatePrefilteredEnvironmentMap();
        }
    }

    void Environment::SetEnvironmentCubemap(const Ref<Texture>& environmentCubemap)
    {
        m_EnvironmentCubemap = environmentCubemap;

        UpdateEnvironmentIrradianceMap();

        m_EnvironmentUniform.HasEnvironmentMap = 1;

        UpdateDescriptorSet();
    }

    void Environment::UpdateDescriptorSet()
    {
        m_DescriptorSet = Renderer::DescriptorSet::Create({
            .Name = "Environment",
            .Layout = Renderer::Context::Instance().GetEnvironmentDescriptorSetLayout()
        });

        vector<Renderer::DescriptorSetWriteInfo> writes;

        m_EnvironmentBuffer->Upload(std::span(reinterpret_cast<u8*>(&m_EnvironmentUniform),
                                              sizeof(EnvironmentUniform)));

        writes.push_back({
            .Type = vk::DescriptorType::eUniformBuffer,
            .Binding = 0,
            .Data = vector<Renderer::DescriptorBufferInfo>{
                {
                    .Buffer = *m_EnvironmentBuffer,
                    .Offset = 0,
                    .Range = sizeof(EnvironmentUniform)
                }
            }
        });

        writes.push_back({
            .Type = vk::DescriptorType::eCombinedImageSampler,
            .Binding = 1,
            .Data = vector<Renderer::DescriptorImageInfo>{
                {
                    .ImageView = *m_EnvironmentCubemap->GetCubemapImageView(),
                    .Sampler = *m_EnvironmentCubemap->GetSampler()
                }
            }
        });

        m_DescriptorSet->UpdateDescriptorSet({
            .Writes = writes
        });
    }

    void Environment::Render(const RenderEnvironmentInfo& info) const
    {
        const uvec2 extent = {info.Target.GetWidth(), info.Target.GetHeight()};

        Renderer::Command::SubmitResource(m_EnvironmentCubemap);
        Renderer::Command::SubmitResource(m_SkyboxMesh);
        Renderer::Command::SubmitResource(m_DescriptorSet);
        Renderer::Command::SubmitResource(m_PrefilteredEnvironmentCubemap);

        info.CommandBuffer.BeginRendering({
            .Extent = extent,
            .Offset = {0, 0},
            .ColorAttachments = {
                {
                    .ImageView = info.TargetView,
                    .LoadOp = vk::AttachmentLoadOp::eLoad,
                    .StoreOp = vk::AttachmentStoreOp::eStore,
                    .ClearValue = {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}}
                }
            },
            .DepthAttachment = {
                Renderer::RenderingAttachmentInfo{
                    .ImageView = info.DepthTargetView,
                    .LoadOp = vk::AttachmentLoadOp::eLoad,
                    .StoreOp = vk::AttachmentStoreOp::eStore,
                    .ClearValue = vk::ClearValue{.depthStencil = {1.0f, 0}}
                }
            },
            .LayerCount = 1,
        });

        info.CommandBuffer.SetScissor({0, 0}, extent);
        info.CommandBuffer.SetViewport({0, 0}, extent);

        info.CommandBuffer.SetScissor({0, 0}, extent);
        info.CommandBuffer.SetViewport({0, 0}, extent);

        info.CommandBuffer.BindPipeline(*m_Pipeline);
        info.CommandBuffer.BindDescriptorSets({
            .Sets = {
                {info.SceneDataDescriptorSet},
                {*m_DescriptorSet},
            },
            .PipelineLayout = *m_PipelineLayout
        });

        info.CommandBuffer.BindVertexBuffer(*m_SkyboxMesh->GetVertexBuffer());
        info.CommandBuffer.BindIndexBuffer(*m_SkyboxMesh->GetIndexBuffer());

        for (const auto& submesh : m_SkyboxMesh->GetSubmeshes())
        {
            info.CommandBuffer.DrawIndexed(submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
        }

        info.CommandBuffer.EndRendering();
    }
}
