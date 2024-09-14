#pragma once

#include "AssetDescriptor.h"
#include "Insight.h"
#include "Library.h"
#include "Assets/Asset.h"
#include "Loaders/AssetLoader.h"

namespace Insight
{
    class RuntimeLibrary : public Library
    {
    public:
        RuntimeLibrary() = default;
        explicit RuntimeLibrary(const path& path);
        ~RuntimeLibrary() override = default;

        Ref<Asset> LoadAsset(AssetHandle handle, const Loaders::AssetLoadOptions& options) override;

        [[nodiscard]] AssetDescriptor& GetAssetDescriptor(const AssetHandle& handle) override;
        [[nodiscard]] bool TryGetAssetDescriptor(const path& assetPath, AssetDescriptor* outDescriptor) override;

        void RegisterAssetLoader(AssetType type, Unique<Loaders::AssetLoader> loader);
        void RegisterInsightAssetLoaders();

        [[nodiscard]] path GetPath() const override { return m_Path; }
        [[nodiscard]] path GetDirectory() const override { return m_Directory; }

    private:
        path m_Path;
        path m_Directory;

        unordered_map<Uuid, AssetDescriptor> m_Assets{};
        unordered_map<AssetType, u32> m_AssetCounts{};
        unordered_map<AssetType, Unique<Loaders::AssetLoader>> m_AssetLoaders{};
    };
}
