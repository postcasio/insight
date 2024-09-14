#pragma once

#include <Insight/AssetDescriptor.h>
#include <Insight/Assets/Texture.h>

#include "Portent.h"


namespace Portent
{
    class EditorResources
    {
    public:
        EditorResources();
        ~EditorResources() = default;

        [[nodiscard]] Ref<Texture> GetAssetTypeIcon(AssetType type) const
        {
            return m_AssetTypeIcons.at(type);
        }

        [[nodiscard]] Ref<Texture> GetGridTexture() const
        {
            return m_GridTexture;
        }

        [[nodiscard]] const vector<Ref<Texture>>& GetHDRITextures() const
        {
            return m_HDRICubemaps;
        }

    private:
        static Ref<Texture> LoadTexture(const path& path, bool repeat = false, vk::Format format = vk::Format::eR8G8B8A8Srgb);
        unordered_map<AssetType, Ref<Texture>> m_AssetTypeIcons;
        Ref<Texture> m_GridTexture;
        vector<Ref<Texture>> m_HDRICubemaps;
    };
}
