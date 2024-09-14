#pragma once

#include "../Math.h"

namespace Insight
{
    struct DirectionalLightComponent
    {
        vec3 Color = {1.0f, 1.0f, 1.0f};
        float Intensity = 1.0f;
        bool CastsShadows = false;

        DirectionalLightComponent() = default;
        DirectionalLightComponent(const DirectionalLightComponent &) = default;
        DirectionalLightComponent(const vec3 &color, const float intensity)
            : Color(color), Intensity(intensity) {}
        DirectionalLightComponent(const vec3 &color, const float intensity, const bool castsShadows)
            : Color(color), Intensity(intensity), CastsShadows(castsShadows) {}
    };
}