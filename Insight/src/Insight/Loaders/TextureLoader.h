#pragma once
#include "AssetLoader.h"
#include "../Insight.h"
#include "../Renderer/Image.h"

namespace Insight::Loaders
{
    static inline const std::pair<vk::Format, string> s_FormatNames[] = {
        {vk::Format::eR8G8B8A8Unorm, "Rgba8Linear"},
        {vk::Format::eR8G8B8A8Srgb, "Rgba8Srgb"},
        {vk::Format::eR16G16B16A16Sfloat, "Rgba16Float"},
        {vk::Format::eR16G16B16Sfloat, "Rgb16Float"},
        {vk::Format::eR32G32B32A32Sfloat, "Rgba32Float"},
    };

    const string& GetFormatName(vk::Format format);
    vk::Format GetFormat(const string& name);

    static inline const std::pair<vk::Filter, string> s_FilterNames[] = {
        {vk::Filter::eNearest, "Nearest"},
        {vk::Filter::eLinear, "Linear"}
    };

    const string& GetFilterName(vk::Filter format);
    vk::Filter GetFilter(const string& name);

    static inline const std::pair<vk::SamplerAddressMode, string> s_SamplerAddressModeNames[] = {
        {vk::SamplerAddressMode::eRepeat, "Repeat"},
        {vk::SamplerAddressMode::eMirroredRepeat, "MirroredRepeat"},
        {vk::SamplerAddressMode::eClampToEdge, "ClampToEdge"},
        {vk::SamplerAddressMode::eClampToBorder, "ClampToBorder"},
        {vk::SamplerAddressMode::eMirrorClampToEdge, "MirrorClampToEdge"}
    };

    const string& GetSamplerAddressModeName(vk::SamplerAddressMode format);
    vk::SamplerAddressMode GetSamplerAddressMode(const string& name);

    class TextureLoader final : public AssetLoader
    {
    public:
        TextureLoader() = default;
        ~TextureLoader() override = default;

        Ref<Asset> Load(const path& texturePath, const AssetLoadOptions& options, std::function<Ref<Asset>(AssetHandle)> loadDependency) override;
    };
}
