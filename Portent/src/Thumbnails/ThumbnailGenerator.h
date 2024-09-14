#pragma once

#include "../Portent.h"
#include <Insight/Assets/Texture.h>

namespace Portent::Thumbnails {

class ThumbnailGenerator {
public:
    ThumbnailGenerator() = default;
    virtual ~ThumbnailGenerator() = default;

    [[nodiscard]] virtual Ref<Texture> GenerateThumbnail(Ref<Asset> asset, uvec2 size) const = 0;
};

}
