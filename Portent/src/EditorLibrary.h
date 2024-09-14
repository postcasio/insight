#pragma once

#include "Portent.h"
#include "Importers/TextureImporter.h"
#include "Insight/Library.h"
#include "Insight/Assets/Texture.h"
#include "Savers/TextureSaver.h"

namespace Portent
{
    enum class EditorLibraryImportStatus
    {
        NotSupported,
        Success,
        InvokeWizard
    };

    struct EditorLibraryImportResult
    {
        EditorLibraryImportStatus Status;
        AssetHandle Handle;
    };

    class EditorLibrary final : public Library
    {
    public:
        EditorLibrary();
        ~EditorLibrary() override = default;

        Ref<Asset> LoadAsset(AssetHandle handle, const Loaders::AssetLoadOptions& options) override;

        [[nodiscard]] AssetDescriptor& GetAssetDescriptor(const AssetHandle& handle) override;
        [[nodiscard]] bool TryGetAssetDescriptor(const path& assetPath, AssetDescriptor* outDescriptor) override;

        void RegisterAssetLoader(AssetType type, Unique<Loaders::AssetLoader> loader);
        void RegisterInsightAssetLoaders();

        [[nodiscard]] path GetPath() const override { return m_Path; }
        [[nodiscard]] path GetDirectory() const override { return m_Directory; }

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

        void Load(const path& path);
        void Persist(const path& path);
        void Persist() { Persist(m_Path); }
        void AddAsset(const AssetDescriptor& descriptor);
        void RemoveAsset(AssetHandle id);

    private:
        path m_Path;
        path m_Directory;

        unordered_map<Uuid, AssetDescriptor> m_Assets{};
        unordered_map<AssetType, u32> m_AssetCounts{};
        unordered_map<AssetType, Unique<Loaders::AssetLoader>> m_AssetLoaders{};

    };
}
