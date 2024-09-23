#include "../Entity.h"
#include "TransformComponent.h"

namespace Insight
{
    void TransformComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<TransformComponent>();
    }
}
