#pragma once
#include "Geometry.h"

namespace Insight
{
    struct SphereGeometryInfo
    {
        float Radius = 1;
        float PhiStart = 0;
        float PhiLength = 2 * glm::pi<float>();
        float ThetaStart = 0;
        float ThetaLength = glm::pi<float>();
        u32 WidthSegments = 32;
        u32 HeightSegments = 16;
    };

    class SphereGeometry final : public Geometry
    {
    public:
        explicit SphereGeometry(const SphereGeometryInfo& info);

    private:

    };
}
