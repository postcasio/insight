#include "../Entity.h"
#include "NameComponent.h"

namespace Insight
{
    void NameComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<NameComponent>();
    }
}
