#pragma once
#include "Geometry.h"

namespace Insight
{
    struct BoxGeometryInfo
    {
        float Width = 1;
        float Height = 1;
        float Depth = 1;
        u32 WidthSegments = 1;
        u32 HeightSegments = 1;
        u32 DepthSegments = 1;
    };

    class BoxGeometry final : public Geometry
    {
    public:
        explicit BoxGeometry(const BoxGeometryInfo& info);

    private:
        void BuildPlane(
            u32 u, u32 v, u32 w,
            float udir, float vdir,
            float width, float height, float depth,
            u32 gridX, u32 gridY,
            u32 materialIndex);
    };
}
