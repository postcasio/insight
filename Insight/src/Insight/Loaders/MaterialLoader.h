#pragma once
#include "AssetLoader.h"

namespace Insight::Loaders
{
    class MaterialLoader final : public AssetLoader
    {
    public:
        MaterialLoader() = default;
        ~MaterialLoader() override = default;

        Ref<Asset> Load(const path& materialPath, const AssetLoadOptions& options, std::function<Ref<Asset>(AssetHandle)> loadDependency) override;
    };
}
