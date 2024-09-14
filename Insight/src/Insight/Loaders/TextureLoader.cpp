#include <stb_image.h>

#include "TextureLoader.h"

#include <tinyexr.h>

#include "../Fs.h"
#include "../Assets/Texture.h"

namespace Insight::Loaders
{
    const string& GetFormatName(const vk::Format format)
    {
        for (const auto& s_FormatName : s_FormatNames)
        {
            if (s_FormatName.first == format)
            {
                return s_FormatName.second;
            }
        }

        return s_FormatNames[0].second;
    }

    vk::Format GetFormat(const string& name)
    {
        for (const auto& s_FormatName : s_FormatNames)
        {
            if (s_FormatName.second == name)
            {
                return s_FormatName.first;
            }
        }

        return vk::Format::eR8G8B8A8Srgb;
    }

    const string& GetFilterName(vk::Filter format)
    {
        for (const auto& s_FilterName : s_FilterNames)
        {
            if (s_FilterName.first == format)
            {
                return s_FilterName.second;
            }
        }

        return s_FilterNames[0].second;
    }

    vk::Filter GetFilter(const string& name)
    {
        for (const auto& s_FilterName : s_FilterNames)
        {
            if (s_FilterName.second == name)
            {
                return s_FilterName.first;
            }
        }

        return vk::Filter::eLinear;
    }

    const string& GetSamplerAddressModeName(vk::SamplerAddressMode format)
    {
        for (const auto& s_SamplerAddressModeName : s_SamplerAddressModeNames)
        {
            if (s_SamplerAddressModeName.first == format)
            {
                return s_SamplerAddressModeName.second;
            }
        }

        return s_SamplerAddressModeNames[0].second;
    }

    vk::SamplerAddressMode GetSamplerAddressMode(const string& name)
    {
        for (const auto& s_SamplerAddressModeName : s_SamplerAddressModeNames)
        {
            if (s_SamplerAddressModeName.second == name)
            {
                return s_SamplerAddressModeName.first;
            }
        }

        return vk::SamplerAddressMode::eRepeat;
    }


    Ref<Asset> TextureLoader::Load(const path& texturePath, const AssetLoadOptions& options, std::function<Ref<Asset>(AssetHandle)> loadDependency)
    {
        json textureInfo = Fs::ReadJsonFile(texturePath);

        path textureFilePath = std::filesystem::canonical(
            texturePath.parent_path() / (textureInfo["Path"].get<string>()));

        INS_ENGINE_INFO("Loading texture {0}", textureFilePath.string());

        vk::Format format = GetFormat(textureInfo["Format"].get<string>());

        i32 texWidth, texHeight, texChannels;

        u8* pixels = nullptr;
        u32 pixelSize;

        if (format == vk::Format::eR32G32B32A32Sfloat)
        {
            const char* err = nullptr;
            float* hdrPixels;
            auto ret = LoadEXR(&hdrPixels, &texWidth, &texHeight, textureFilePath.c_str(), &err);

            if (ret != TINYEXR_SUCCESS)
            {
                if (err)
                {
                    throw std::runtime_error(fmt::format("Failed to load texture image {0}: {1}", textureFilePath.string(), err));
                }

                throw std::runtime_error(fmt::format("Failed to load texture image {0}", textureFilePath.string()));
            }

            pixels = reinterpret_cast<u8*>(hdrPixels);
            pixelSize = sizeof(float) * 4;
        }
        else
        {
            pixels = stbi_load(textureFilePath.c_str(), &texWidth, &texHeight, &texChannels,
                                        STBI_rgb_alpha);
            pixelSize = sizeof(u8) * 4;
        }
        const size_t imageSize = texWidth * texHeight * pixelSize;

        if (!pixels)
        {
            throw std::runtime_error(fmt::format("Failed to load texture image {0}", textureFilePath.string()));
        }

        auto texture = Texture::Create({
            .Name = textureInfo["Path"].get<string>(),
            .Width = static_cast<u32>(texWidth),
            .Height = static_cast<u32>(texHeight),
            .MipLevels = textureInfo["MipLevels"].get<u32>(),
            .Data = std::span(pixels, imageSize),
            .Format = format,
            .Filter = GetFilter(textureInfo["Filter"].get<string>()),
            .AddressMode = GetSamplerAddressMode(textureInfo["AddressMode"].get<string>()),
            .KeepData = options.CacheData
        });

        if (!options.CacheData)
        {
            delete pixels;
        }

        return texture;
    }
}
