#include "GBufferPass.h"

#include "../Image.h"
#include "../Context.h"
#include "../../Assets/Material.h"
#include "../../Scene.h"
#include "../../Components/TransformComponent.h"
#include "../../Components/StaticMeshComponent.h"
#include "../Command.h"

namespace Insight::Renderer
{
    GBufferPass::GBufferPass()
    {
        m_SamplerDescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "GBuffer Samplers",
            .Bindings = {
                {
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .binding = 0,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment
                },
                {
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .binding = 1,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment
                },
                {
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .binding = 2,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment
                },
                {
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .binding = 3,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment
                }
            }
        });

        m_Sampler = Sampler::Create({
            .Name = "GBuffer Sampler",
            .MagFilter = vk::Filter::eNearest,
            .MinFilter = vk::Filter::eNearest,
            .MipmapMode = vk::SamplerMipmapMode::eNearest,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

    }

    void GBufferPass::CreateResources()
    {
        m_PositionImage = Image::Create({
            .Name = "Position",
            .Extent = {m_Extent, 1},
            .Format = vk::Format::eR32G32B32A32Sfloat,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_PositionView = ImageView::Create({
            .Name = "Position",
            .Image = *m_PositionImage
        });

        m_NormalImage = Image::Create({
            .Name = "Normal",
            .Extent = {m_Extent, 1},
            .Format = vk::Format::eR32G32B32A32Sfloat,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_NormalView = ImageView::Create({
            .Name = "Normal",
            .Image = *m_NormalImage
        });

        m_OrmImage = Image::Create({
            .Name = "Orm",
            .Extent = {m_Extent, 1},
            .Format = vk::Format::eR8G8B8A8Unorm,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_OrmView = ImageView::Create({
            .Name = "Orm",
            .Image = *m_OrmImage
        });

        m_AlbedoImage = Image::Create({
            .Name = "Albedo",
            .Extent = {m_Extent, 1},
            .Format = Context::Instance().GetOutputFormat(),
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_AlbedoView = ImageView::Create({
            .Name = "Albedo",
            .Image = *m_AlbedoImage
        });

        m_DepthImage = Image::Create({
            .Name = "Depth",
            .Extent = {m_Extent, 1},
            .Format = vk::Format::eD32Sfloat,
            .Usage = vk::ImageUsageFlagBits::eDepthStencilAttachment,
            .Filter = vk::Filter::eNearest,
            .Type = vk::ImageType::e2D
        });

        m_DepthImageView = ImageView::Create({
            .Name = "Depth",
            .Image = *m_DepthImage
        });

        m_SamplerDescriptorSet = DescriptorSet::Create({
            .Name = "GBuffer Samplers",
            .Layout = *m_SamplerDescriptorSetLayout
        });

        m_SamplerDescriptorSet->UpdateDescriptorSet({
            .Writes = {
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 0,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_PositionView,
                            .Sampler = *m_Sampler
                        },
                        {
                            .ImageView = *m_NormalView,
                            .Sampler = *m_Sampler
                        },
                        {
                            .ImageView = *m_OrmView,
                            .Sampler = *m_Sampler
                        },
                        {
                            .ImageView = *m_AlbedoView,
                            .Sampler = *m_Sampler
                        }
                    }
                }
            }
        });
    }

    void GBufferPass::Execute(const GBufferPassExecuteInfo& info)
    {
        Command::SubmitResource(m_PositionImage);
        Command::SubmitResource(m_PositionView);
        Command::SubmitResource(m_NormalImage);
        Command::SubmitResource(m_NormalView);
        Command::SubmitResource(m_AlbedoImage);
        Command::SubmitResource(m_AlbedoView);
        Command::SubmitResource(m_OrmImage);
        Command::SubmitResource(m_OrmView);
        Command::SubmitResource(m_DepthImage);
        Command::SubmitResource(m_DepthImageView);
        Command::SubmitResource(m_SamplerDescriptorSet);

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_PositionImage,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_NormalImage,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_OrmImage,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_AlbedoImage,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        info.CommandBuffer.BeginRendering({
            .Extent = m_Extent,
            .Offset = {0, 0},
            .ColorAttachments = GetAttachments(),
            .DepthAttachment = GetDepthAttachment(),
            .LayerCount = 1,
        });

        info.CommandBuffer.SetScissor({0, 0}, m_Extent);
        info.CommandBuffer.SetViewport({0, 0}, m_Extent);

        ModelPushConstants modelPushConstants = {
            .Model = glm::mat4(1.0f),
            .Normal = glm::mat4(1.0f)
        };

        info.Scene.GetRegistry().view<TransformComponent, StaticMeshComponent>().each(
            [this, info, modelPushConstants](auto entity, TransformComponent& transform,
                                             StaticMeshComponent& mesh) mutable
            {
                auto staticMesh = mesh.Mesh;

                if (!staticMesh)
                {
                    return;
                }

                auto materialCount = staticMesh->GetMaterials().size();

                Command::SubmitResource(staticMesh);
                Command::SubmitResources(staticMesh->GetMaterials());

                info.CommandBuffer.BindVertexBuffer(*staticMesh->GetVertexBuffer());
                info.CommandBuffer.BindIndexBuffer(*staticMesh->GetIndexBuffer());

                modelPushConstants.Model = transform.GetTransform();
                modelPushConstants.Normal = glm::transpose(glm::inverse(modelPushConstants.Model));

                for (auto submesh : staticMesh->GetSubmeshes())
                {
                    auto material = staticMesh->GetMaterial(submesh.MaterialIndex % materialCount);

                    if (material)
                    {
                        material->Bind(info.SceneDataDescriptorSet, info.CommandBuffer, modelPushConstants);

                        info.CommandBuffer.DrawIndexed(submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
                    }
                }
            }
        );

        info.CommandBuffer.EndRendering();

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_PositionImage,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_NormalImage,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_OrmImage,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = *m_AlbedoImage,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });
    }

    vector<RenderingAttachmentInfo> GBufferPass::GetAttachments() const
    {
        return {
            {
                .ImageView = *m_PositionView,
                .LoadOp = vk::AttachmentLoadOp::eClear,
                .StoreOp = vk::AttachmentStoreOp::eStore,
                .ClearValue = vk::ClearValue{
                    .color = std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})
                }
            },
            {
                .ImageView = *m_NormalView,
                .LoadOp = vk::AttachmentLoadOp::eClear,
                .StoreOp = vk::AttachmentStoreOp::eStore,
                .ClearValue = vk::ClearValue{
                    .color = std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})
                }
            },
            {
                .ImageView = *m_OrmView,
                .LoadOp = vk::AttachmentLoadOp::eClear,
                .StoreOp = vk::AttachmentStoreOp::eStore,
                .ClearValue = vk::ClearValue{
                    .color = std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})
                }
            },
            {
                .ImageView = *m_AlbedoView,
                .LoadOp = vk::AttachmentLoadOp::eClear,
                .StoreOp = vk::AttachmentStoreOp::eStore,
                .ClearValue = vk::ClearValue{
                    .color = std::array<float, 4>({0.0f, 0.0f, 0.0f, 1.0f})
                }
            }
        };
    }

    RenderingAttachmentInfo GBufferPass::GetDepthAttachment() const
    {
        return
        {
            .ImageView = *m_DepthImageView,
            .LoadOp = vk::AttachmentLoadOp::eClear,
            .StoreOp = vk::AttachmentStoreOp::eStore,
            .ClearValue = vk::ClearValue{.depthStencil = {1.0f, 0}}
        };
    }
}
