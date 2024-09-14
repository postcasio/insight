#pragma once
#include "AssetType.h"
#include "Uuid.h"

namespace Insight
{
    using AssetHandle = Uuid;

    struct AssetDescriptor
    {
        AssetHandle Id = AssetHandle::Null;
        AssetType Type = AssetType::Unknown;
        path Path;
    };
} // Insight
