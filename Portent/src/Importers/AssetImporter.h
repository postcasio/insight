#pragma once

#include "../Portent.h"
#include <Insight/Assets/Asset.h>

namespace Portent::Importers
{
    class AssetImporter
    {
    public:
        AssetImporter() = default;
        virtual ~AssetImporter() = default;

        virtual AssetHandle Import(const path& path) = 0;
        virtual bool IsSupported(const path& path) = 0;
    };
}
