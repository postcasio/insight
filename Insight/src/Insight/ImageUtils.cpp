#include "ImageUtils.h"

namespace Insight {
    Ref<Texture> CreateTexture(const CreateTextureInfo& info)
    {
        auto pixels = std::vector<u8>(info.Width * info.Height * 4, 0);

        for (size_t i = 0; i < pixels.size(); i += 4)
        {
            pixels[i + 0] = static_cast<u8>(info.ClearColor.r * 255);
            pixels[i + 1] = static_cast<u8>(info.ClearColor.g * 255);
            pixels[i + 2] = static_cast<u8>(info.ClearColor.b * 255);
            pixels[i + 3] = static_cast<u8>(info.ClearColor.a * 255);
        }

        return Texture::Create({
            .Name = info.Name,
            .Width = info.Width,
            .Height = info.Height,
            .MipLevels = CalculateMipLevels(info.Width, info.Height),
            .Data = std::span(pixels.data(), pixels.size()),
            .Format = vk::Format::eR8G8B8A8Unorm,
            .Filter = vk::Filter::eLinear,
            .AddressMode = vk::SamplerAddressMode::eRepeat,
        });
    }
} // Insight