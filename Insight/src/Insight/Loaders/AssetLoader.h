#pragma once

#include "../Assets/Asset.h"

namespace Insight::Loaders
{
    struct AssetLoadOptions
    {
        bool CacheData = false;
    };

    class AssetLoader
    {
    public:
        AssetLoader() = default;
        virtual ~AssetLoader() = default;

        virtual Ref<Asset> Load(const path& assetPath, const AssetLoadOptions& options, std::function<Ref<Asset>(AssetHandle)> loadDependency) = 0;
    };
}
