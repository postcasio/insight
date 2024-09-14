#include "CommandBuffer.h"

#include "Context.h"
#include "Utils.h"


namespace Insight::Renderer
{
    CommandBuffer::CommandBuffer(const vk::CommandBufferLevel level) :
        m_Level(level)
    {
        const vk::CommandBufferAllocateInfo allocInfo{
            .commandPool = Context::Instance().GetCommandPool().GetVkCommandPool(),
            .level = m_Level,
            .commandBufferCount = 1,
        };

        m_VkCommandBuffer = Context::Instance().GetVkDevice().allocateCommandBuffers(allocInfo)[0];
    }

    void CommandBuffer::Reset() const
    {
        m_VkCommandBuffer.reset();
    }

    void CommandBuffer::Begin(vk::CommandBufferUsageFlags flags) const
    {
        m_VkCommandBuffer.begin({
            .flags = flags
        });
    }

    vk::RenderingAttachmentInfo BeginRendering_CreateAttachment(const RenderingAttachmentInfo& info)
    {
        return {
            .imageView = info.ImageView.GetVkImageView(),
            .imageLayout = Utils::GetFormatAttachmentImageLayout(info.ImageView.GetFormat()),
            .clearValue = info.ClearValue,
            .loadOp = info.LoadOp,
            .storeOp = info.StoreOp,
            .resolveMode = vk::ResolveModeFlagBits::eNone
        };
    }

    void CommandBuffer::BeginRendering(const RenderingInfo& info) const
    {
        vector<vk::RenderingAttachmentInfo> colorAttachments;
        colorAttachments.reserve(info.ColorAttachments.size());

        for (auto attachment : info.ColorAttachments)
        {
            colorAttachments.push_back(BeginRendering_CreateAttachment(attachment));
        }

        vk::RenderingAttachmentInfo depthAttachment;
        bool hasDepthAttachment = false;
        if (info.DepthAttachment.has_value())
        {
            depthAttachment = BeginRendering_CreateAttachment(info.DepthAttachment.value());
            hasDepthAttachment = true;
        }

        vk::RenderingAttachmentInfo stencilAttachment;
        bool hasStencilAttachment = false;
        if (info.StencilAttachment.has_value())
        {
            stencilAttachment = BeginRendering_CreateAttachment(info.StencilAttachment.value());
            hasStencilAttachment = true;
        }

        // const auto renderingInfo = static_cast<VkRenderingInfo>(vk::RenderingInfo {
        const auto renderingInfo = vk::RenderingInfo{
            .flags = info.Flags,
            .layerCount = info.LayerCount,
            .renderArea = {info.Offset.x, info.Offset.y, info.Extent.x, info.Extent.y},
            .viewMask = info.ViewMask,
            .colorAttachmentCount = static_cast<u32>(colorAttachments.size()),
            .pColorAttachments = colorAttachments.data(),
            .pDepthAttachment = hasDepthAttachment ? &depthAttachment : nullptr,
            .pStencilAttachment = hasStencilAttachment ? &stencilAttachment : nullptr,
        };

        m_VkCommandBuffer.beginRendering(renderingInfo);
    }

    void CommandBuffer::EndRendering() const
    {
        m_VkCommandBuffer.endRendering();
    }

    void CommandBuffer::PushConstants(const PushConstantsInfo& info) const
    {
        m_VkCommandBuffer.pushConstants(info.PipelineLayout.GetVkPipelineLayout(), info.StageFlags, info.Offset,
                                        info.Size, info.Data);
    }

    void CommandBuffer::BindDescriptorSets(const DescriptorSetBindInfo& info) const
    {
        vector<vk::DescriptorSet> descriptorSets;
        descriptorSets.reserve(info.Sets.size());

        for (auto& descriptorSet : info.Sets)
        {
            descriptorSets.push_back(descriptorSet.Set.GetVkDescriptorSet());
        }

        m_VkCommandBuffer.bindDescriptorSets(info.PipelineBindPoint, info.PipelineLayout.GetVkPipelineLayout(),
                                             info.FirstSet, descriptorSets, nullptr);
    }

    void CommandBuffer::DrawFullscreenTriangle() const
    {
        m_VkCommandBuffer.draw(3, 1, 0, 0);
    }

    void CommandBuffer::Draw(const u32 vertexCount, const u32 instanceCount, const u32 firstVertex,
                             const u32 firstInstance) const
    {
        m_VkCommandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void CommandBuffer::DrawIndexed(const u32 indexCount, const u32 instanceCount, const u32 firstIndex,
                                    const i32 vertexOffset, const u32 firstInstance) const
    {
        m_VkCommandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }

    void CommandBuffer::BindPipeline(const DynamicGraphicsPipeline& pipeline) const
    {
        m_VkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetVkPipeline());
    }

    void CommandBuffer::BindPipeline(const GraphicsPipeline& pipeline) const
    {
        m_VkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.GetVkPipeline());
    }

    void CommandBuffer::SetScissor(const ivec2 offset, const uvec2 extent) const
    {
        m_VkCommandBuffer.setScissor(0, {vk::Rect2D{{offset.x, offset.y}, {extent.x, extent.y}}});
    }

    void CommandBuffer::SetViewport(const ivec2 offset, const uvec2 extent) const
    {
        m_VkCommandBuffer.setViewport(0, {
                                          vk::Viewport{
                                              static_cast<float>(offset.x), static_cast<float>(offset.y),
                                              static_cast<float>(extent.x), static_cast<float>(extent.y), 0.0f, 1.0f
                                          }
                                      });
    }

    void CommandBuffer::PipelineBarrier(const ImageBarrier& barrier) const
    {
        const auto oldLayout = barrier.Image.GetLayout(barrier.BaseMipLevel);
        const auto newLayout = barrier.NewLayout;

        const vk::ImageMemoryBarrier imageMemoryBarrier{
            .image = barrier.Image.GetVkImage(),
            .oldLayout = oldLayout,
            .newLayout = newLayout,
            .subresourceRange = {
                Utils::GetAspectFlags(barrier.Image.GetFormat()),
                barrier.BaseMipLevel,
                barrier.MipLevelCount,
                barrier.BaseLayer,
                barrier.LayerCount
            },
            .srcAccessMask = Utils::GetAccessMask(oldLayout),
            .dstAccessMask = Utils::GetAccessMask(newLayout),
        };

        m_VkCommandBuffer.pipelineBarrier(
            Utils::GetSourceStageMask(oldLayout),
            Utils::GetDestinationStageMask(newLayout),
            vk::DependencyFlags{},
            0,
            nullptr,
            0,
            nullptr,
            1,
            &imageMemoryBarrier
        );

        barrier.Image.SetLayout(barrier.BaseMipLevel, barrier.MipLevelCount, newLayout);
    }

    void CommandBuffer::End() const
    {
        m_VkCommandBuffer.end();
    }

    void CommandBuffer::BeginRenderPass(RenderPass& renderPass, Framebuffer& framebuffer) const
    {
        auto renderExtent = framebuffer.GetExtent();

        const vector<vk::ClearValue> clearValues = {
            {vk::ClearColorValue{std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}}}
        };

        const vk::RenderPassBeginInfo renderPassInfo{
            .renderPass = renderPass.GetVkRenderPass(),
            .framebuffer = framebuffer.GetVkFramebuffer(),
            .renderArea = {
                .offset = {0, 0},
                .extent = {renderExtent.x, renderExtent.y}
            },
            .clearValueCount = static_cast<u32>(clearValues.size()),
            .pClearValues = clearValues.data(),
        };

        m_VkCommandBuffer.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

        const auto& fbAttachments = framebuffer.GetAttachments();
        const auto& rpAttachments = renderPass.GetAttachments();

        for (auto i = 0; i < fbAttachments.size(); i++)
        {
            const auto& [ImageView] = fbAttachments[i];

            ImageView.GetImage().SetLayout(rpAttachments[i].finalLayout);
        }
    }

    void CommandBuffer::EndRenderPass() const
    {
        m_VkCommandBuffer.endRenderPass();
    }

    CommandBuffer::~CommandBuffer()
    {
        Context::Instance().GetVkDevice().freeCommandBuffers(
            Context::Instance().GetCommandPool().GetVkCommandPool(),
            1, &m_VkCommandBuffer
        );
    }

    void CommandBuffer::BindVertexBuffer(const Buffer& buffer) const
    {
        m_VkCommandBuffer.bindVertexBuffers(0, buffer.GetVkBuffer(), {0});
    }

    void CommandBuffer::BindIndexBuffer(const Buffer& buffer) const
    {
        m_VkCommandBuffer.bindIndexBuffer(buffer.GetVkBuffer(), 0, vk::IndexType::eUint32);
    }

    void CommandBuffer::CopyBufferToImage(const Buffer& buffer, const Image& image) const
    {
        const auto extent = image.GetExtent();

        const vk::BufferImageCopy region = {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource.aspectMask = Utils::GetAspectFlags(image.GetFormat()),
            .imageSubresource.mipLevel = 0,
            .imageSubresource.baseArrayLayer = 0,
            .imageSubresource.layerCount = 1,
            .imageOffset = {0, 0, 0},
            .imageExtent = {extent.x, extent.y, extent.z }
        };

        m_VkCommandBuffer.copyBufferToImage(
            buffer.GetVkBuffer(),
            image.GetVkImage(),
            vk::ImageLayout::eTransferDstOptimal,
            1,
            &region);
    }

    void CommandBuffer::CopyImageToBuffer(const Image& image, const Buffer& buffer) const
    {
        const auto extent = image.GetExtent();

        const vk::BufferImageCopy region = {
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource.aspectMask = Utils::GetAspectFlags(image.GetFormat()),
            .imageSubresource.mipLevel = 0,
            .imageSubresource.baseArrayLayer = 0,
            .imageSubresource.layerCount = 1,
            .imageOffset = {0, 0, 0},
            .imageExtent = {extent.x, extent.y, extent.z }
        };

        m_VkCommandBuffer.copyImageToBuffer(image.GetVkImage(), vk::ImageLayout::eTransferSrcOptimal, buffer.GetVkBuffer(), 1, &region);
    }

    void CommandBuffer::BlitImage(const BlitImageInfo& info) const
    {
        vk::Offset3D sourceOffset = { info.SourceOffset.x, info.SourceOffset.y, info.SourceOffset.z };
        vk::Offset3D sourceExtent = { info.SourceExtent.x, info.SourceExtent.y, info.SourceExtent.z };

        vk::Offset3D destinationOffset = { info.DestinationOffset.x, info.DestinationOffset.y, info.DestinationOffset.z };
        vk::Offset3D destinationExtent = { info.DestinationExtent.x, info.DestinationExtent.y, info.DestinationExtent.z };

        vk::ImageBlit blitInfo {
            .srcOffsets = {{sourceOffset, sourceExtent }},
            .dstOffsets = {{destinationOffset, destinationExtent }},
            .srcSubresource = {
                .aspectMask = Utils::GetAspectFlags(info.SourceImage.GetFormat()),
                .mipLevel = info.SourceMipLevel,
                .baseArrayLayer = 0,
                .layerCount = 1
            },
            .dstSubresource = {
                .aspectMask = Utils::GetAspectFlags(info.DestinationImage.GetFormat()),
                .mipLevel = info.DestinationMipLevel,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        m_VkCommandBuffer.blitImage(
            info.SourceImage.GetVkImage(),
            vk::ImageLayout::eTransferSrcOptimal,
            info.DestinationImage.GetVkImage(),
            vk::ImageLayout::eTransferDstOptimal,
            1,
            &blitInfo,
            vk::Filter::eLinear
        );
    }
}
