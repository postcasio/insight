#include "EditorResources.h"

#include <stb_image.h>
#include <tinyexr.h>

namespace Portent {
    EditorResources::EditorResources()
    {
        for (auto& assetType : s_AssetTypes)
        {
            m_AssetTypeIcons[assetType] = LoadTexture("Editor/AssetIcons/" + GetAssetTypeName(assetType) + ".png");
        }

        m_GridTexture = LoadTexture("Editor/Textures/GridBasic.png", true);

        vector<Ref<Texture>> hdriTextures = {
            LoadTexture("Editor/HDRI/DayEnvironmentHDRI015_2K-HDR.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/golden_bay_2k.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/studio_small_09_2k.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/test.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/zwartkops_straight_afternoon_2k.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/autumn_field_puresky_2k.exr", false, vk::Format::eR32G32B32A32Sfloat),
            LoadTexture("Editor/HDRI/kloofendal_48d_partly_cloudy_puresky_2k.exr", false, vk::Format::eR32G32B32A32Sfloat),
        };

        for (auto& texture : hdriTextures)
        {
            m_HDRICubemaps.push_back(texture->EquirectangularToCubemap());
        }
    }

    Ref<Texture> EditorResources::LoadTexture(const path& path, bool repeat, vk::Format format)
    {
        i32 texWidth, texHeight, texChannels;
        u8* pixels;
        u32 pixelSize;
        u32 mipLevels;

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