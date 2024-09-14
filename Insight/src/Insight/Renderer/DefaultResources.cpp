#include "DefaultResources.h"

#include <stb_image.h>
#include <tinyexr.h>

#include "../Assets/Texture.h"

namespace Insight::Renderer
{
    void DefaultResources::Initialize()
    {
        m_WhiteTexture = CreateTexture(vec4(1.0f));
        m_BlackTexture = CreateTexture(vec4(0.0f));
        m_WhiteCubeTexture = m_WhiteTexture->EquirectangularToCubemap();
        m_BlackCubeTexture = m_BlackTexture->EquirectangularToCubemap();

        m_BrdfLut = CreateTexture("Engine/Textures/BRDF_LUT.exr", false, vk::Format::eR32G32B32A32Sfloat);
    }

    void DefaultResources::Dispose()
    {
        m_WhiteTexture.reset();
        m_BlackTexture.reset();
        m_WhiteCubeTexture.reset();
        m_BlackCubeTexture.reset();
        m_BrdfLut.reset();
    }

    Ref<Texture> DefaultResources::CreateTexture(vec4 color)
    {
        auto texture = Texture::Create({
            .Name = "Color Texture",
            .Width = 1,
            .Height = 1,
            .Format = vk::Format::eR8G8B8A8Unorm,
            .Data = std::span(reinterpret_cast<u8*>(&color), sizeof(vec4)),
            .MipLevels = 1,
            .Filter = vk::Filter::eLinear,
            .AddressMode = vk::SamplerAddressMode::eRepeat,
            .Type = TextureType::Texture2D
        });

        return texture;
    }

    Ref<Texture> DefaultResources::CreateTexture(const path& path, bool repeat, vk::Format format)
    {
        i32 texWidth, texHeight, texChannels;
        u8* pixels;
        u32 pixelSize;
        u32 mipLevels = 0;

        if (format == vk::Format::eR32G32B32A32Sfloat)
        {
            const char* err = nullptr;
            float* hdrPixels;
            auto ret = LoadEXR(&hdrPixels, &texWidth, &texHeight, path.c_str(), &err);

            if (ret != TINYEXR_SUCCESS)
            {
                if (err)
                {
                    throw std::runtime_error(fmt::format("Failed to load texture image {0}: {1}", path.string(), err));
                }

                throw std::runtime_error(fmt::format("Failed to load texture image {0}", path.string()));
            }

            pixels = reinterpret_cast<u8*>(hdrPixels);
            pixelSize = sizeof(float) * 4;
            mipLevels = 1;
        }
        else
        {
            pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels,
                                        STBI_rgb_alpha);
            pixelSize = sizeof(u8) * 4;
            mipLevels = CalculateMipLevels(texWidth, texHeight);
        }

        const size_t imageSize = texWidth * texHeight * pixelSize;

        if (!pixels)
        {
            throw std::runtime_error(fmt::format("Failed to load texture image {0}", path.string()));
        }

        return Texture::Create({
            .Name = path,
            .Width = static_cast<u32>(texWidth),
            .Height = static_cast<u32>(texHeight),
            .MipLevels = mipLevels,
            .Data = std::span(pixels, imageSize),
            .Format = format,
            .Filter = vk::Filter::eLinear,
            .AddressMode = repeat ? vk::SamplerAddressMode::eRepeat : vk::SamplerAddressMode::eClampToEdge,
            .KeepData = false
        });
    }
}
