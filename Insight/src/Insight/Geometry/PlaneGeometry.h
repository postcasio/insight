#pragma once
#include "Geometry.h"

namespace Insight
{
    struct PlaneGeometryInfo
    {
        float Width = 1;
        float Height = 1;
        u32 WidthSegments = 1;
        u32 HeightSegments = 1;
        vec2 UvScale = {1, 1};
    };

    class PlaneGeometry final : public Geometry
    {
    public:
        explicit PlaneGeometry(const PlaneGeometryInfo& info);

    private:

    };
}
