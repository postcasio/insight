#pragma once

#include "../Math.h"

namespace Insight
{
    class Entity;

    struct PointLightComponent
    {
        static inline const Uuid ComponentId = Uuid {"04ce881b-f310-453c-8416-886d7675dddd"};
        static inline const string ComponentName = "PointLightComponent";
        static void AddFunction(Entity* entity);
        static inline const JSClass* JSClass = nullptr;

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