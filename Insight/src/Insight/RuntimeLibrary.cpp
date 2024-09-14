#include "RuntimeLibrary.h"

#include "Fs.h"
#include "Loaders/MaterialLoader.h"
#include "Loaders/TextureLoader.h"

namespace Insight {
    RuntimeLibrary::RuntimeLibrary(const path& path) : Library(), m_Path(path)
    {
        INS_ENGINE_INFO("Loading library from {0}", path.string());

        m_Directory = path.parent_path();

        json libraryData = Fs::ReadJsonFile(path);

        // get length of asset array
        const u32 assetCount = libraryData["Assets"].size();

        m_Assets.reserve(assetCount);

        for (const auto& asset : libraryData["Assets"])
        {
            AssetDescriptor descriptor;

            descriptor.Id = Uuid(asset["Id"].get<string>());
            descriptor.Type = GetAssetType(asset["Type"].get<string>());
            descriptor.Path = asset["Path"].get<string>();

            m_Assets[descriptor.Id] = descriptor;

            if (!m_AssetCounts.contains(descriptor.Type))
            {
                m_AssetCounts[descriptor.Type] = 1;
            }
            else
            {
                m_AssetCounts[descriptor.Type]++;
            }
        }

        INS_ENGINE_INFO("Loaded {0} asset descriptors", assetCount);

        for (const auto& [type, count] : m_AssetCounts)
        {
            INS_ENGINE_INFO("\t{0}: {1}", GetAssetTypeName(type), count);
        }

        RegisterInsightAssetLoaders();
    }

    Ref<Asset> RuntimeLibrary::LoadAsset(const AssetHandle handle, const Loaders::AssetLoadOptions& options)
    {
        AssetDescriptor descriptor = m_Assets[handle];

        if (m_AssetLoaders.contains(descriptor.Type))
        {
            return m_AssetLoaders[descriptor.Type]->Load(m_Directory / descriptor.Path, options, [this, options](AssetHandle handle) { return LoadAsset(handle, options); });
        }

        throw std::runtime_error(fmt::format("No loader for asset type {0}", GetAssetTypeName(descriptor.Type)));
    }

    AssetDescriptor& RuntimeLibrary::GetAssetDescriptor(const AssetHandle& handle)
    {
        return m_Assets[handle];
    }

    bool RuntimeLibrary::TryGetAssetDescriptor(const path& assetPath, AssetDescriptor* outDescriptor)
    {
        // get path relative to library directory
        const string relativePath = assetPath.lexically_proximate(m_Directory);

        for (auto& descriptor : m_Assets | std::views::values)
        {
            if (descriptor.Path == relativePath)
            {
                *outDescriptor = descriptor;
                return true;
            }
        }

        return false;
    }

    void RuntimeLibrary::RegisterAssetLoader(AssetType type, Unique<Loaders::AssetLoader> loader)
    {
        m_AssetLoaders[type] = std::move(loader);
    }

    void RuntimeLibrary::RegisterInsightAssetLoaders()
    {
        RegisterAssetLoader(AssetType::Texture, CreateUnique<Loaders::TextureLoader>());
        RegisterAssetLoader(AssetType::Material, CreateUnique<Loaders::MaterialLoader>());
    }
} // Insight