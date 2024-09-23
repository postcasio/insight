#include "../Entity.h"
#include "EnvironmentComponent.h"

namespace Insight
{
    void EnvironmentComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<EnvironmentComponent>();
    }
}
