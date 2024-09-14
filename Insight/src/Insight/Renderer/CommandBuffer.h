#pragma once

#include "Buffer.h"
#include "DescriptorSet.h"
#include "DynamicGraphicsPipeline.h"
#include "Framebuffer.h"
#include "GraphicsPipeline.h"
#include "ImageBarrier.h"
#include "RenderPass.h"
#include "Vulkan.h"
#include "ImageView.h"


namespace Insight::Renderer
{
    struct PushConstantsInfo
    {
        PipelineLayout& PipelineLayout;
        vk::ShaderStageFlags StageFlags;
        u32 Offset;
        u32 Size;
        const void* Data;
    };

    struct RenderingAttachmentInfo
    {
        ImageView& ImageView;
        vk::AttachmentLoadOp LoadOp;
        vk::AttachmentStoreOp StoreOp;
        vk::ClearValue ClearValue;
    };

    struct RenderingInfo
    {
        vk::RenderingFlags Flags{};
        ivec2 Offset = {0, 0};
        uvec2 Extent;
        u32 LayerCount = 1;
        u32 ViewMask = 0;
        vector<RenderingAttachmentInfo> ColorAttachments{};
        optional<RenderingAttachmentInfo> DepthAttachment{};
        optional<RenderingAttachmentInfo> StencilAttachment{};
    };

    struct DescriptorSetBindSetInfo
    {
        DescriptorSet& Set;
    };

    struct DescriptorSetBindInfo
    {
        vector<DescriptorSetBindSetInfo> Sets;
        u32 FirstSet = 0;
        vk::PipelineBindPoint PipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        PipelineLayout& PipelineLayout;
    };

    struct BlitImageInfo
    {
        Image &SourceImage;
        Image &DestinationImage;
        u32 SourceMipLevel;
        u32 DestinationMipLevel;
        ivec3 SourceOffset;
        ivec3 DestinationOffset;
        ivec3 SourceExtent;
        ivec3 DestinationExtent;
    };

    class CommandBuffer
    {
    public:
        static Unique<CommandBuffer> Create(vk::CommandBufferLevel level = {})
        {
            return CreateUnique<CommandBuffer>(level);
        }

        ~CommandBuffer();
        void BindVertexBuffer(const Buffer& buffer) const;
        void BindIndexBuffer(const Buffer& buffer) const;

        explicit CommandBuffer(vk::CommandBufferLevel level);

        [[nodiscard]] vk::CommandBuffer GetVkCommandBuffer() const { return m_VkCommandBuffer; }

        void Reset() const;

        void Begin(vk::CommandBufferUsageFlags flags = {}) const;
        void End() const;

        void BeginRenderPass(RenderPass& renderPass, Framebuffer& framebuffer) const;
        void EndRenderPass() const;

        void BeginRendering(const RenderingInfo& info) const;
        void EndRendering() const;

        void PushConstants(const PushConstantsInfo& info) const;

        void Draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) const;
        void DrawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 vertexOffset, u32 firstInstance) const;

        void BindPipeline(const DynamicGraphicsPipeline& pipeline) const;
        void BindPipeline(const GraphicsPipeline& pipeline) const;

        void SetScissor(ivec2 offset, uvec2 extent) const;
        void SetViewport(ivec2 offset, uvec2 extent) const;

        void BindDescriptorSets(const DescriptorSetBindInfo& info) const;
        void DrawFullscreenTriangle() const;

        void PipelineBarrier(const ImageBarrier& barrier) const;

        void CopyBufferToImage(const Buffer& buffer, const Image& image) const;
        void CopyImageToBuffer(const Image& image, const Buffer& buffer) const;
        void BlitImage(const BlitImageInfo& info) const;

    protected:


    private:
        vk::CommandBufferLevel m_Level;
        vk::CommandBuffer m_VkCommandBuffer;
    };
}
