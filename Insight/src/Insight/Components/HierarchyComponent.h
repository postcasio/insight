#pragma once

#include "../Insight.h"
#include "../Entity.h"

namespace Insight
{
    class Entity;

    struct HierarchyComponent
    {
        static inline const Uuid ComponentId = Uuid {"ea2636c0-4276-4680-9687-38369c663217"};
        static inline const string ComponentName = "HierarchyComponent";
        static void AddFunction(Entity* entity);
        static inline const JSClass* JSClass = nullptr;

        EntityHandle PrevSibling = Entity::NullHandle;
        EntityHandle NextSibling = Entity::NullHandle;
        EntityHandle FirstChild = Entity::NullHandle;
        EntityHandle LastChild = Entity::NullHandle;
        EntityHandle Parent = Entity::NullHandle;
        u32 Children = 0;

        HierarchyComponent() = default;
        HierarchyComponent(const HierarchyComponent&) = default;
    };
}
