#include "TextureImporter.h"

#include <stb_image.h>
#include <tinyexr.h>

#include "../Editor.h"
#include "Insight/Loaders/TextureLoader.h"


namespace Portent::Importers
{
    AssetHandle TextureImporter::Import(const path& path)
    {
        auto& library = Editor::Instance().GetProject().GetLibrary();

        const auto insightTexturePath = library.GetDirectory() / (path.string() + ".InsightTexture");

        AssetDescriptor descriptor;

        if (library.TryGetAssetDescriptor(insightTexturePath, &descriptor))
        {
            return descriptor.Id;
        }


        int width, height, channels;
        vk::Format format;

        if (path.extension() == ".exr")
        {
            const char* err = nullptr;
            float* hdrPixels;
            auto ret = LoadEXR(&hdrPixels, &width, &height, path.c_str(), &err);

            if (ret != TINYEXR_SUCCESS)
            {
                if (err)
                {
                    throw std::runtime_error(fmt::format("Failed to load texture image {0}: {1}", path.string(), err));
                }

                throw std::runtime_error(fmt::format("Failed to load texture image {0}", path.string()));
            }


            free(hdrPixels);
            format = vk::Format::eR32G32B32A32Sfloat;
        }
        else
        {
            const auto file = fopen(path.c_str(), "rb");
            stbi_info_from_file(file, &width, &height, &channels);
            format = vk::Format::eR8G8B8A8Srgb;
            fclose(file);
        }

        json textureData;

        const auto id = Uuid::CreateNew();
        const auto texturePath = path.filename();


        textureData["Id"] = id.ToString();
        textureData["Path"] = texturePath;
        textureData["Filter"] = Loaders::GetFilterName(vk::Filter::eLinear);
        textureData["Format"] = Loaders::GetFormatName(format);
        textureData["AddressMode"] = Loaders::GetSamplerAddressModeName(vk::SamplerAddressMode::eRepeat);
        textureData["MipLevels"] = static_cast<u32>(std::floor(std::log2(std::max(width, height)))) + 1;

        Fs::WriteJsonFile(insightTexturePath, textureData);

        descriptor = {
            .Id = id,
            .Path = insightTexturePath.lexically_proximate(library.GetDirectory()),
            .Type = AssetType::Texture
        };

        library.AddAsset(descriptor);

        return id;
    }

    bool TextureImporter::IsSupported(const path& path)
    {
        auto extension = path.extension().string();

        std::ranges::transform(extension, extension.begin(), ::tolower);

        auto supportedExtensions = {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".exr"};

        return std::ranges::find(supportedExtensions, extension) != supportedExtensions.end();
    }
}
