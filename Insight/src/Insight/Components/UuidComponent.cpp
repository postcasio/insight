#include "../Entity.h"
#include "UuidComponent.h"

namespace Insight
{
    void UuidComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<UuidComponent>();
    }
}
