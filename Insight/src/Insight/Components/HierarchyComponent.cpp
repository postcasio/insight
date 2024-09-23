#include "../Entity.h"
#include "HierarchyComponent.h"

namespace Insight
{
    void HierarchyComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<HierarchyComponent>();
    }
}
