#include "AssetType.h"

namespace Insight {
    const string& GetAssetTypeName(AssetType type)
    {
        return s_AssetTypeNames[static_cast<u32>(type)];
    }

    AssetType GetAssetType(const string& name)
    {
        for (u32 i = 0; i < s_AssetTypeCount; i++)
        {
            if (s_AssetTypeNames[i] == name)
            {
                return static_cast<AssetType>(i);
            }
        }

        return AssetType::Unknown;
    }
}