#pragma once

#include "AssetDescriptor.h"
#include "Insight.h"
#include "Assets/Asset.h"
#include "Loaders/AssetLoader.h"

namespace Insight
{
    class Library
    {
    public:
        Library() = default;
        virtual ~Library() = default;

        virtual Ref<Asset> LoadAsset(AssetHandle handle, const Loaders::AssetLoadOptions& options) = 0;

        template<Derived<Asset> T>
        Ref<T> LoadAsset(const AssetHandle handle)
        {
            return std::static_pointer_cast<T>(LoadAsset(handle, {}));
        }

        template<Derived<Asset> T>
        Ref<T> LoadAsset(const AssetHandle handle, const Loaders::AssetLoadOptions &options)
        {
            return std::static_pointer_cast<T>(LoadAsset(handle, options));
        }

        [[nodiscard]] virtual AssetDescriptor& GetAssetDescriptor(const AssetHandle& handle) = 0;
        [[nodiscard]] virtual bool TryGetAssetDescriptor(const path& assetPath, AssetDescriptor* outDescriptor) = 0;

        [[nodiscard]] virtual path GetPath() const = 0;
        [[nodiscard]] virtual path GetDirectory() const = 0;
    };
}
