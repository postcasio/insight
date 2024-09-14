#pragma once

#include "../Math.h"

namespace Insight
{
    struct PointLightComponent
    {
        vec3 Color = {1.0f, 1.0f, 1.0f};
        float Intensity = 1.0f;
        float Range = 1024.0f;
        bool CastsShadows = false;

        PointLightComponent() = default;
        PointLightComponent(const PointLightComponent &) = default;
        PointLightComponent(const vec3 &color, const float intensity, const float range)
            : Color(color), Intensity(intensity), Range(range) {}
        PointLightComponent(const vec3 &color, const float intensity, const float range, const bool castsShadows)
            : Color(color), Intensity(intensity), Range(range), CastsShadows(castsShadows) {}
    };
}