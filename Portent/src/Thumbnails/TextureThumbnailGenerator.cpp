#include "TextureThumbnailGenerator.h"

#include "../Editor.h"


namespace Portent::Thumbnails
{
    Ref<Texture> TextureThumbnailGenerator::GenerateThumbnail(const Ref<Asset> asset, uvec2 size) const
    {
        if (asset->GetType() != AssetType::Texture)
        {
            return nullptr;
        }

        auto texture = static_pointer_cast<Texture>(asset);

        // create a new image with the desired size

        auto thumbnail = Renderer::Image::Create({
            .Name = "Thumbnail: " + texture->GetName(),
            .Extent = {size.x, size.y, 1},
            .Format = vk::Format::eR8G8B8A8Srgb,
            .Usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eLinear,
            .MipLevels = 1
        });

        const auto commandBuffer = Renderer::CommandBuffer::Create();
        commandBuffer->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        commandBuffer->PipelineBarrier({
            .Image = *thumbnail,
            .NewLayout = vk::ImageLayout::eTransferDstOptimal
        });

        commandBuffer->PipelineBarrier({
            .Image = *texture->GetImage(),
            .NewLayout = vk::ImageLayout::eTransferSrcOptimal
        });

        commandBuffer->BlitImage({
            .SourceOffset = {0, 0, 0},
            .SourceExtent = {texture->GetWidth(), texture->GetHeight(), 1},
            .SourceImage = *texture->GetImage(),
            .DestinationOffset = {0, 0, 0},
            .DestinationExtent = {size.x, size.y, 1},
            .DestinationImage = *thumbnail,
        });

        commandBuffer->PipelineBarrier({
            .Image = *texture->GetImage(),
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        commandBuffer->PipelineBarrier({
            .Image = *thumbnail,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        commandBuffer->End();

        Renderer::Context::Instance().SubmitImmediateCommands(*commandBuffer);

        return Texture::Create(thumbnail, {
            .Filter = vk::Filter::eNearest,
            .Format = thumbnail->GetFormat(),
            .AddressMode = vk::SamplerAddressMode::eClampToEdge,
            .Name = thumbnail->GetName(),
            .MipLevels = thumbnail->GetMipLevels()
        });
    }
}
