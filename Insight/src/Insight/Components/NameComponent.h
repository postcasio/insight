#pragma once

#include "../Insight.h"

namespace Insight
{
    class Entity;

    struct NameComponent
    {
        static inline const Uuid ComponentId = Uuid {"df1b520e-fc82-48c3-97af-e82397926a02"};
        static inline const string ComponentName = "NameComponent";
        static void AddFunction(Entity* entity);
        static inline const JSClass* JSClass = nullptr;

        string Name;

        NameComponent() = default;
        NameComponent(const NameComponent&) = default;

        explicit NameComponent(const string& name) : Name(name)
        {
        }
    };
}
