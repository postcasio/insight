#pragma once

#include "../Insight.h"
#include "../Assets/Prefab.h"

namespace Insight
{
    struct PrefabComponent
    {
        Ref<Prefab> Prefab;

        PrefabComponent() = default;
        PrefabComponent(const PrefabComponent&) = default;

        explicit PrefabComponent(const Ref<Insight::Prefab>& prefab) : Prefab(prefab)
        {
        }
    };
}
