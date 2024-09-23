#include "../Entity.h"
#include "StaticMeshComponent.h"

namespace Insight
{
    void StaticMeshComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<StaticMeshComponent>();
    }
}
