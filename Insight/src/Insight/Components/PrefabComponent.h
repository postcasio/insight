#pragma once

#include "../Insight.h"
#include "../Assets/Prefab.h"

namespace Insight
{
    class Entity;

    struct PrefabComponent
    {
        static inline const Uuid ComponentId = Uuid {"dffbbdcc-91ac-49fa-a24c-19aa9cfa3434"};
        static inline const string ComponentName = "PrefabComponent";
        static void AddFunction(Entity* entity);

        Ref<Prefab> Prefab;

        PrefabComponent() = default;
        PrefabComponent(const PrefabComponent&) = default;

        explicit PrefabComponent(const Ref<Insight::Prefab>& prefab) : Prefab(prefab)
        {
        }
    };
}
