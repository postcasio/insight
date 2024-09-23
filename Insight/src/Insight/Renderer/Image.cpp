#include "Image.h"

#include "Buffer.h"
#include "Context.h"
#include "DebugMarkers.h"
#include "EquirectangularToCubemapRenderer.h"
#include "IrradianceRenderer.h"
#include "PrefilteredEnvironmentRenderer.h"
#include "../Application.h"
#include "../Assets/Texture.h"

namespace Insight::Renderer
{
    Image::Image(const vk::Image vkImage, const ImageInfo& info) :
        m_Name(info.Name),
        m_Extent(info.Extent),
        m_MipLevels(info.MipLevels),
        m_Layers(info.Layers),
        m_Format(info.Format),
        m_Type(info.Type),
        m_Usage(info.Usage),
        m_Filter(info.Filter),
        m_Managed(false),
        m_VkImage(vkImage)
    {
        m_Layouts.resize(m_MipLevels, vk::ImageLayout::eUndefined);

        DebugMarkers::MarkImage(m_VkImage, m_Name);
    }

    Image::Image(const ImageInfo& info) :
        m_Name(info.Name),
        m_Extent(info.Extent),
        m_MipLevels(info.MipLevels),
        m_Layers(info.Layers),
        m_Format(info.Format),
        m_Type(info.Type),
        m_Usage(info.Usage),
        m_Filter(info.Filter),
        m_Managed(true)
    {
        m_Layouts.resize(m_MipLevels, vk::ImageLayout::eUndefined);

        vk::ImageCreateFlags flags;

        if (m_Layers == 6)
        {
            flags |= vk::ImageCreateFlagBits::eCubeCompatible;
        }

        VkImageCreateInfo imageCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = static_cast<VkImageType>(m_Type),
            .format = static_cast<VkFormat>(m_Format),
            .extent = {m_Extent.x, m_Extent.y, m_Extent.z},
            .mipLevels = m_MipLevels,
            .arrayLayers = m_Layers,
            .samples = static_cast<VkSampleCountFlagBits>(vk::SampleCountFlagBits::e1),
            .tiling = static_cast<VkImageTiling>(vk::ImageTiling::eOptimal),
            .usage = static_cast<VkImageUsageFlags>(m_Usage),
            .sharingMode = static_cast<VkSharingMode>(vk::SharingMode::eExclusive),
            .initialLayout = static_cast<VkImageLayout>(vk::ImageLayout::eUndefined),
            .flags = static_cast<VkImageCreateFlags>(flags)
        };

        VmaAllocationCreateInfo allocationCreateInfo{
            .usage = VMA_MEMORY_USAGE_AUTO,
            .requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .pool = VK_NULL_HANDLE,
        };

        VkImage image;

        VK_RAW_ASSERT(vmaCreateImage(
                          Context::Instance().GetAllocator(),
                          &imageCreateInfo,
                          &allocationCreateInfo,
                          &image,
                          &m_VmaAllocation,
                          &m_VmaAllocationInfo), "Failed to create image");

        m_VkImage = image;

        vmaSetAllocationName(
            Context::Instance().GetAllocator(),
            m_VmaAllocation,
            m_Name.c_str());

        DebugMarkers::MarkImage(m_VkImage, m_Name);
    }

    Image::~Image()
    {
        if (m_Managed)
        {
            vmaDestroyImage(
                Context::Instance().GetAllocator(),
                m_VkImage,
                m_VmaAllocation);
        }

        if (m_UiTextureID)
        {
            Ui::Context::DestroyDescriptorSet(m_UiTextureID);
            m_UiSampler.reset();
            m_UiImageView.reset();
        }
    }

    ImTextureID Image::GetImTextureId()
    {
        if (!m_UiTextureID)
        {
            m_UiImageView = ImageView::Create({
                .Name = m_Name + " (UI)",
                .Image = *this,
                .MipLevels = 1
            });

            m_UiSampler = Sampler::Create({
                .Name = m_Name + " (UI)",
            });

            m_UiTextureID = Ui::Context::CreateDescriptorSet(
                *m_UiSampler, *m_UiImageView);
        }

        return m_UiTextureID;
    }

    ImTextureID Image::GetCubemapFaceImTextureId(u32 face)
    {
        if (m_UiCubemapFaceImageViews.empty())
        {
            if (!m_UiSampler)
            {
                m_UiSampler = Sampler::Create({
                    .Name = m_Name + " (UI)",
                });
            }

            for (u32 i = 0; i < 6; i++)
            {
                m_UiCubemapFaceImageViews.push_back(Renderer::ImageView::Create({
                    .Name = m_Name + " (UI)",
                    .Image = *this,
                    .MipLevels = 1,
                    .ArrayLayers = 1,
                    .BaseArrayLayer = i
                }));

                m_UiCubemapFaceTextureIDs.push_back(Ui::Context::CreateDescriptorSet(
                    *m_UiSampler, *m_UiCubemapFaceImageViews[i]));
            }
        }

        return m_UiCubemapFaceTextureIDs[face];
    }

    ImTextureID Image::GetCubemapFaceMipImTextureId(u32 face, u32 mip)
    {
        if (m_UiCubemapFaceMipTextureIDs.empty())
        {
            if (!m_UiSampler)
            {
                m_UiSampler = Sampler::Create({
                    .Name = m_Name + " (UI)",
                });
            }

            for (u32 i = 0; i < 6; i++)
            {
                vector<Ref<ImageView>> views{};
                vector<ImTextureID> textureIds{};

                for (u32 j = 0; j < m_MipLevels; j++)
                {
                    const auto view = Renderer::ImageView::Create({
                        .Name = m_Name + " (UI)",
                        .Image = *this,
                        .MipLevels = 1,
                        .BaseMipLevel = j,
                        .ArrayLayers = 1,
                        .BaseArrayLayer = i
                    });

                    ImTextureID textureId = Ui::Context::CreateDescriptorSet(*m_UiSampler, *view);

                    views.push_back(view);
                    textureIds.push_back(textureId);

                }

                m_UiCubemapFaceMipImageViews.push_back(views);
                m_UiCubemapFaceMipTextureIDs.push_back(textureIds);
            }
        }

        return m_UiCubemapFaceMipTextureIDs[face][mip];
    }

    void Image::GenerateMipmaps(const CommandBuffer& commandBuffer)
    {
        ImageBarrier barrier{
            .Image = *this,
            .NewLayout = vk::ImageLayout::eTransferSrcOptimal,
            .BaseMipLevel = 0
        };

        u32 mipWidth = m_Extent.x;
        u32 mipHeight = m_Extent.y;

        for (u32 i = 1; i < m_MipLevels; i++)
        {
            barrier.NewLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.BaseMipLevel = i - 1;

            commandBuffer.PipelineBarrier(barrier);

            commandBuffer.BlitImage({
                .SourceImage = *this,
                .DestinationImage = *this,
                .SourceMipLevel = i - 1,
                .DestinationMipLevel = i,
                .SourceOffset = {0, 0, 0},
                .DestinationOffset = {0, 0, 0},
                .SourceExtent = {mipWidth, mipHeight, 1},
                .DestinationExtent = {mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1}
            });

            barrier.NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

            commandBuffer.PipelineBarrier(barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.BaseMipLevel = m_MipLevels - 1;
        barrier.NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal;

        commandBuffer.PipelineBarrier(barrier);
    }

    Ref<Image> Image::EquirectangularToCubemap()
    {
        EquirectangularToCubemapRenderer renderer{vk::Format::eR16G16B16A16Sfloat};

        const auto cubemap = Create({
            .Name = m_Name + " (Cubemap)",
            .Extent = {512, 512, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Filter = m_Filter,
            .Layers = 6,
            .Usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment,
            .MipLevels = CalculateMipLevels(512, 512),
            .Type = vk::ImageType::e2D
        });

        renderer.Render({
            .EquirectangularMap = *this,
            .Target = *cubemap
        });

        return cubemap;
    }

    void Image::Upload(std::span<u8> span)
    {
        auto stagingBuffer = Buffer::Create({
            .Name = m_Name + " (Upload)",
            .Size = span.size(),
            .Usage = vk::BufferUsageFlagBits::eTransferSrc,
        });

        stagingBuffer->Upload(span);

        auto commandBuffer = CommandBuffer::Create();

        commandBuffer->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffer->PipelineBarrier(ImageBarrier{
            .Image = *this,
            .NewLayout = vk::ImageLayout::eTransferDstOptimal,
            .BaseMipLevel = 0,
            .MipLevelCount = m_MipLevels
        });
        commandBuffer->CopyBufferToImage(*stagingBuffer, *this);

        if (m_MipLevels > 1)
        {
            GenerateMipmaps(*commandBuffer);
        }
        else
        {
            commandBuffer->PipelineBarrier(ImageBarrier{
                .Image = *this,
                .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
                .LayerCount = m_Layers,
                .MipLevelCount = 1
            });
        }

        commandBuffer->End();

        Context::Instance().SubmitImmediateCommands(*commandBuffer);
    }

    std::span<u8> Image::Download()
    {
        auto buffer = Buffer::Create({
            .Name = m_Name + " (Download)",
            .Size = m_Extent.x * m_Extent.y * 4,
            .Usage = vk::BufferUsageFlagBits::eTransferDst,
        });

        auto layout = GetLayout(0);

        auto commandBuffer = CommandBuffer::Create();

        commandBuffer->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        commandBuffer->PipelineBarrier(ImageBarrier{
            .Image = *this,
            .NewLayout = vk::ImageLayout::eTransferSrcOptimal
        });

        commandBuffer->CopyImageToBuffer(*this, *buffer);

        commandBuffer->PipelineBarrier(ImageBarrier{
            .Image = *this,
            .NewLayout = layout
        });

        commandBuffer->End();

        Context::Instance().SubmitImmediateCommands(*commandBuffer);

        return buffer->Download();
    }

    Ref<Image> Image::CreateIrradianceMap()
    {
        IrradianceRenderer renderer{vk::Format::eR16G16B16A16Sfloat};

        const auto cubemap = Create({
            .Name = m_Name + " (Irradiance)",
            .Extent = {32, 32, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Filter = m_Filter,
            .Layers = 6,
            .Usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment,
            .MipLevels = 1,
            .Type = vk::ImageType::e2D
        });

        renderer.Render({
            .Cubemap = *this,
            .Target = *cubemap
        });

        return cubemap;
    }

    Ref<Image> Image::CreatePrefilteredEnvironmentMap()
    {
        PrefilteredEnvironmentRenderer renderer{vk::Format::eR16G16B16A16Sfloat};

        const auto cubemap = Create({
            .Name = m_Name + " (Prefiltered)",
            .Extent = {512, 512, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Filter = m_Filter,
            .Layers = 6,
            .Usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eColorAttachment,
            .MipLevels = CalculateMipLevels(512, 512),
            .Type = vk::ImageType::e2D
        });

        renderer.Render({
            .Cubemap = *this,
            .Target = *cubemap
        });

        return cubemap;
    }
}
