#include "EditorLibrary.h"

#include "Insight/Fs.h"
#include "Insight/Loaders/MaterialLoader.h"
#include "Insight/Loaders/StaticMeshLoader.h"
#include "Insight/Loaders/TextureLoader.h"

namespace Portent {
    EditorLibrary::EditorLibrary() : Library()
    {
        RegisterInsightAssetLoaders();
    }

    Ref<Asset> EditorLibrary::LoadAsset(const AssetHandle handle, const Loaders::AssetLoadOptions& options)
    {
        AssetDescriptor descriptor = m_Assets[handle];

        if (m_AssetLoaders.contains(descriptor.Type))
        {
            Ref<Asset> asset = m_AssetLoaders[descriptor.Type]->Load(m_Directory / descriptor.Path, options,
                [this, options](AssetHandle handle) { return LoadAsset(handle, options); });

            asset->SetHandle(handle);
            asset->SetDescriptor(descriptor);

            return asset;
        }

        throw std::runtime_error(fmt::format("No loader for asset type {0}", GetAssetTypeName(descriptor.Type)));
    }

    AssetDescriptor& EditorLibrary::GetAssetDescriptor(const AssetHandle& handle)
    {
        return m_Assets[handle];
    }

    bool EditorLibrary::TryGetAssetDescriptor(const path& assetPath, AssetDescriptor* outDescriptor)
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

    void EditorLibrary::RegisterAssetLoader(AssetType type, Unique<Loaders::AssetLoader> loader)
    {
        m_AssetLoaders[type] = std::move(loader);
    }

    void EditorLibrary::RegisterInsightAssetLoaders()
    {
        RegisterAssetLoader(AssetType::Texture, CreateUnique<Loaders::TextureLoader>());
        RegisterAssetLoader(AssetType::Material, CreateUnique<Loaders::MaterialLoader>());
        RegisterAssetLoader(AssetType::StaticMesh, CreateUnique<Loaders::StaticMeshLoader>());
    }

    void EditorLibrary::Load(const path& path)
    {
        m_Path = path;

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
    }

    void EditorLibrary::Persist(const path& path)
    {
        json libraryData;

        vector<json> assets;

        for (const auto& [id, descriptor] : m_Assets)
        {
            json asset;
            asset["Id"] = descriptor.Id.ToString();
            asset["Type"] = GetAssetTypeName(descriptor.Type);
            asset["Path"] = descriptor.Path;

            assets.push_back(asset);
        }

        libraryData["Assets"] = assets;

        Fs::WriteJsonFile(path, libraryData);
    }

    void EditorLibrary::AddAsset(const AssetDescriptor& descriptor)
    {
        m_Assets[descriptor.Id] = descriptor;

        Persist();
    }

    void EditorLibrary::RemoveAsset(const AssetHandle id)
    {
        const AssetDescriptor descriptor = m_Assets[id];

        m_AssetCounts[descriptor.Type]--;
        m_Assets.erase(id);

        Persist();
    }
} // Insight