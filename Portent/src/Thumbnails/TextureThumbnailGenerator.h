#pragma once
#include "ThumbnailGenerator.h"


namespace Portent::Thumbnails
{
    class TextureThumbnailGenerator final : public ThumbnailGenerator
    {
    public:
        [[nodiscard]] Ref<Texture> GenerateThumbnail(Ref<Asset> asset, uvec2 size) const override;
    };
}
