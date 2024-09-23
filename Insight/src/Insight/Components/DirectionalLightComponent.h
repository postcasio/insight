#pragma once

#include "../Math.h"

namespace Insight
{
    class Entity;

    struct DirectionalLightComponent
    {
        static inline const Uuid ComponentId = Uuid {"8f63ffff-28d0-4193-b6d0-f3d2234cf13c"};
        static inline const string ComponentName = "DirectionalLightComponent";
        static void AddFunction(Entity* entity);

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