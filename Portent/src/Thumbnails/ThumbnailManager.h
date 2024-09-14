#pragma once

#include "../Portent.h"
#include <Insight/Assets/Texture.h>

#include "ThumbnailGenerator.h"
#include "Insight/AssetDescriptor.h"
#include "Insight/Library.h"

namespace Portent::Thumbnails
{
    struct ThumbnailInfo
    {
        AssetHandle Handle;
        bool Ready;
        Ref<Texture> Texture;
    };

    class ThumbnailManager
    {
    public:
        ThumbnailManager();
        ~ThumbnailManager();

        bool TryGetThumbnail(AssetHandle handle, ThumbnailInfo* outThumbnail) const;

        bool TryGetThumbnailTexture(AssetHandle handle, Ref<Texture>* outTexture) const;

        bool TryLoadThumbnail(AssetHandle handle, ThumbnailInfo* outThumbnail);
        Ref<Texture> GetThumbnailTexture(AssetHandle handle);
        bool CanGenerateThumbnail(AssetType type) const;

        void Initialize(const Library& library);

    private:
        ThumbnailInfo GenerateThumbnail(AssetHandle uuid);

        unordered_map<AssetType, ThumbnailGenerator*> m_Generators;
        unordered_map<AssetHandle, ThumbnailInfo> m_Thumbnails{};
        uvec2 m_ThumbnailSize = {128, 128};
        path m_Directory;
    };
}
