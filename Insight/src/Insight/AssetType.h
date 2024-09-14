#pragma once

#include "Insight.h"

namespace Insight
{
    enum class AssetType : u32
    {
        Unknown = 0,
        Directory = 1,
        Texture = 2,
        Material = 3,
        StaticMesh = 4,
        Prefab = 5,
        Data = 6
    };

    static inline constexpr AssetType s_AssetTypes[] = {
        AssetType::Unknown,
        AssetType::Directory,
        AssetType::Texture,
        AssetType::Material,
        AssetType::StaticMesh,
        AssetType::Prefab,
        AssetType::Data
    };

    static inline const string s_AssetTypeNames[] = {
        "Insight.Unknown",
        "Insight.Directory",
        "Insight.Texture",
        "Insight.Material",
        "Insight.StaticMesh",
        "Insight.Prefab",
        "Insight.Data"
    };

    static inline constexpr u32 s_AssetTypeCount = sizeof(s_AssetTypeNames) / sizeof(string);

    const string& GetAssetTypeName(AssetType type);
    AssetType GetAssetType(const string& name);
}
