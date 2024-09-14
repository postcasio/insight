#pragma once
#include "ThumbnailGenerator.h"


namespace Portent::Thumbnails
{
    class MaterialThumbnailGenerator final : public ThumbnailGenerator
    {
    public:
        [[nodiscard]] Ref<Texture> GenerateThumbnail(Ref<Asset> asset, uvec2 size) const override;
    };
}
