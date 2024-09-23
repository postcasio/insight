#pragma once

#include "../Insight.h"
#include "../Environment.h"

namespace Insight
{
    class Entity;

    struct EnvironmentComponent
    {
        static inline const Uuid ComponentId = Uuid {"b5b64534-24e5-416f-8e96-bf426da371c4"};
        static inline const string ComponentName = "EnvironmentComponent";
        static void AddFunction(Entity* entity);
        static inline const JSClass* JSClass = nullptr;

        Ref<Environment> Environment = CreateRef<Insight::Environment>();

        EnvironmentComponent() = default;
        ~EnvironmentComponent() = default;
        EnvironmentComponent(const EnvironmentComponent&) = default;
    };
}
