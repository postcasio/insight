#pragma once
#include "AssetImporter.h"


namespace Portent::Importers
{
    class TextureImporter final : public AssetImporter
    {
    public:
        ~TextureImporter() override = default;
        AssetHandle Import(const path& path) override;
        bool IsSupported(const path& path) override;
    };
}
