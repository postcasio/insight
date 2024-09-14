#include "ThumbnailManager.h"

#include <stb_image.h>

#include "TextureThumbnailGenerator.h"
#include "../Editor.h"
#include <stb_image_write.h>

#include "MaterialThumbnailGenerator.h"

namespace Portent::Thumbnails
{
    ThumbnailManager::ThumbnailManager()
    {
        m_Generators[AssetType::Texture] = static_cast<ThumbnailGenerator*>(new TextureThumbnailGenerator());
        m_Generators[AssetType::Material] = static_cast<ThumbnailGenerator*>(new MaterialThumbnailGenerator());
    }

    ThumbnailManager::~ThumbnailManager()
    {
        for (auto& [type, generator] : m_Generators)
        {
            delete generator;
        }
    }

    bool ThumbnailManager::TryGetThumbnail(const AssetHandle handle, ThumbnailInfo* outThumbnail) const
    {
        if (m_Thumbnails.contains(handle))
        {
            *outThumbnail = m_Thumbnails.at(handle);
            return true;
        }

        return false;
    }

    bool ThumbnailManager::TryGetThumbnailTexture(const AssetHandle handle, Ref<Texture>* outTexture) const
    {
        if (m_Thumbnails.contains(handle))
        {
            auto info = m_Thumbnails.at(handle);

            if (!info.Ready)
            {
                return false;
            }

            *outTexture = info.Texture;

            return true;
        }

        return false;
    }

    bool ThumbnailManager::TryLoadThumbnail(AssetHandle handle, ThumbnailInfo* outThumbnail)
    {
        const auto thumbnailPath = m_Directory / fmt::format("{0}.png", handle.ToString());
        if (Fs::FileExists(thumbnailPath))
        {
            i32 texWidth, texHeight, texChannels;
            stbi_uc* pixels = stbi_load(thumbnailPath.c_str(), &texWidth, &texHeight, &texChannels,
                                        STBI_rgb_alpha);

            const size_t imageSize = texWidth * texHeight * 4;

            if (!pixels)
            {
                throw std::runtime_error(fmt::format("Failed to load texture image {0}", thumbnailPath.string()));
            }

            auto texture = Texture::Create({
                .Name = thumbnailPath,
                .Width = static_cast<u32>(texWidth),
                .Height = static_cast<u32>(texHeight),
                .MipLevels = 1,
                .Data = std::span(pixels, imageSize),
                .Format = vk::Format::eR8G8B8A8Srgb,
                .Filter = vk::Filter::eLinear,
                .AddressMode = vk::SamplerAddressMode::eClampToEdge,
                .KeepData = false
            });

            delete pixels;

            m_Thumbnails[handle] = {
                handle,
                true,
                texture
            };

            outThumbnail = &m_Thumbnails[handle];

            return true;
        }

        return false;
    }

    Ref<Texture> ThumbnailManager::GetThumbnailTexture(const AssetHandle handle)
    {
        ThumbnailInfo info = {Uuid::Null, false, nullptr};

        if (!TryGetThumbnail(handle, &info))
        {
            if (!TryLoadThumbnail(handle, &info))
            {
                info = GenerateThumbnail(handle);

                INS_CLIENT_INFO("Generating thumbnail for {0}", handle.ToString());

                m_Thumbnails[handle] = info;
            }
        }

        return info.Ready ? info.Texture : nullptr;
    }

    bool ThumbnailManager::CanGenerateThumbnail(AssetType type) const
    {
        return m_Generators.contains(type);
    }

    void ThumbnailManager::Initialize(const Library& library)
    {
        m_Directory = library.GetDirectory() / ".ThumbnailCache";

        if (!Fs::FileExists(m_Directory))
        {
            Fs::CreateDirectory(m_Directory);
        }

        m_Thumbnails.clear();
    }

    ThumbnailInfo ThumbnailManager::GenerateThumbnail(const AssetHandle uuid)
    {
        const AssetDescriptor descriptor = Editor::Instance().GetProject().GetLibrary().GetAssetDescriptor(uuid);

        for (auto& [type, generator] : m_Generators)
        {
            if (type == descriptor.Type)
            {
                const Ref<Asset> asset = Editor::Instance().GetProject().GetLibrary().LoadAsset(uuid, {});

                Ref<Texture> texture = generator->GenerateThumbnail(asset, m_ThumbnailSize);

                std::span<u8> data = texture->GetImage()->Download();

                stbi_write_png((m_Directory / fmt::format("{0}.png", uuid.ToString())).c_str(),
                           texture->GetWidth(),
                           texture->GetHeight(),
                           4,
                           data.data(),
                           0);

                delete data.data();

                return {
                    uuid,
                    true,
                    texture
                };
            }
        }

        return {
            uuid, false, nullptr
        };
    }
}
