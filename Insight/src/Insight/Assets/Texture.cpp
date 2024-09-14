#include "Texture.h"
#include "../Renderer/CommandBuffer.h"
#include "../Renderer/Context.h"

namespace Insight
{
    u32 CalculateMipLevels(const u32 width, const u32 height)
    {
        return static_cast<u32>(std::floor(std::log2(std::max(width, height)))) + 1;
    }

    Texture::Texture(const TextureInfo& info) : Asset(AssetType::Texture), m_Name(info.Name), m_Filter(info.Filter),
                                                m_AddressMode(info.AddressMode), m_Type(info.Type)
    {
        if (info.KeepData)
        {
            m_Data = info.Data;
        }

        m_MaximumSupportedMipLevels = CalculateMipLevels(info.Width, info.Height);;

        m_Image = Renderer::Image::Create({
            .Name = info.Name,
            .Extent = uvec3{info.Width, info.Height, 1u},
            .Filter = info.Filter,
            .Format = info.Format,
            .Usage = vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eTransferSrc,
            .MipLevels = info.MipLevels,
            .Layers = m_Type == TextureType::TextureCubemap ? 6u : 1u,
            .Type = vk::ImageType::e2D
        });

        m_Image->Upload(info.Data);

        for (u32 i = 0; i < m_Image->GetLayers(); i++)
        {
            m_LayerImageViews.push_back(Renderer::ImageView::Create({
                .Name = info.Name,
                .Image = *m_Image,
                .MipLevels = info.MipLevels,
                .ArrayLayers = 1,
                .BaseArrayLayer = i
            }));
        }

        if (m_Type == TextureType::TextureCubemap)
        {
            m_CubemapImageView = Renderer::ImageView::Create({
                .Name = info.Name,
                .Image = *m_Image,
                .MipLevels = info.MipLevels,
                .ArrayLayers = 6,
                .BaseArrayLayer = 0,
                .ViewType = vk::ImageViewType::eCube
            });
        }

        m_Sampler = Renderer::Sampler::Create({
            .Name = info.Name,
            .AddressModeU = info.AddressMode,
            .AddressModeV = info.AddressMode,
            .AddressModeW = info.AddressMode,
            .MinFilter = info.Filter,
            .MagFilter = info.Filter,
            .MaxLod = static_cast<float>(info.MipLevels)
        });
    }

    Texture::Texture(const Ref<Renderer::Image>& image, const TextureFromImageInfo& info) : Asset(AssetType::Texture),
        m_Image(image), m_Name(info.Name), m_AddressMode(info.AddressMode), m_Filter(info.Filter), m_Type(info.Type)
    {
        m_MaximumSupportedMipLevels = CalculateMipLevels(image->GetWidth(), image->GetHeight());

        for (u32 i = 0; i < m_Image->GetLayers(); i++)
        {
            m_LayerImageViews.push_back(Renderer::ImageView::Create({
                .Name = info.Name,
                .Image = *m_Image,
                .MipLevels = info.MipLevels,
                .ArrayLayers = 1,
                .BaseArrayLayer = i
            }));
        }

        if (m_Type == TextureType::TextureCubemap)
        {
            m_CubemapImageView = Renderer::ImageView::Create({
                .Name = info.Name,
                .Image = *m_Image,
                .MipLevels = info.MipLevels,
                .ArrayLayers = 6,
                .BaseArrayLayer = 0,
                .ViewType = vk::ImageViewType::eCube
            });
        }

        m_Sampler = Renderer::Sampler::Create({
            .Name = info.Name,
            .AddressModeU = info.AddressMode,
            .AddressModeV = info.AddressMode,
            .AddressModeW = info.AddressMode,
            .MinFilter = info.Filter,
            .MagFilter = info.Filter,
            .MaxLod = static_cast<float>(info.MipLevels)
        });
    }

    Texture::~Texture()
    {
        if (m_Data.has_value())
        {
            delete m_Data->data();
        }
    }

    Ref<Texture> Texture::EquirectangularToCubemap()
    {
        auto image = m_Image->EquirectangularToCubemap();

        return Create(image, {
                          .Name = m_Name + " (Cubemap)",
                          .Filter = image->GetFilter(),
                          .Format = image->GetFormat(),
                          .AddressMode = vk::SamplerAddressMode::eClampToEdge,
                          .MipLevels = 1,
                          .Type = TextureType::TextureCubemap
                      });
    }

    Ref<Texture> Texture::CreateIrradianceMap()
    {
        auto image = m_Image->CreateIrradianceMap();

        return Create(image, {
                          .Name = m_Name + " (Irradiance)",
                          .Filter = image->GetFilter(),
                          .Format = image->GetFormat(),
                          .AddressMode = vk::SamplerAddressMode::eClampToEdge,
                          .MipLevels = 1,
                          .Type = TextureType::TextureCubemap
                      });
    }

    Ref<Texture> Texture::CreatePrefilteredEnvironmentMap()
    {
        auto image = m_Image->CreatePrefilteredEnvironmentMap();

        return Create(image, {
                          .Name = m_Name + " (Prefiltered Environment)",
                          .Filter = image->GetFilter(),
                          .Format = image->GetFormat(),
                          .AddressMode = vk::SamplerAddressMode::eClampToEdge,
                          .MipLevels = image->GetMipLevels(),
                          .Type = TextureType::TextureCubemap
                      });
    }
}
