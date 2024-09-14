#pragma once

#include "../Insight.h"
#include "../Entity.h"

namespace Insight
{
    struct HierarchyComponent
    {
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
