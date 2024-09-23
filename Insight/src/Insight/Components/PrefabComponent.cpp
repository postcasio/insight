#include "../Entity.h"
#include "PrefabComponent.h"

namespace Insight
{
    void PrefabComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<PrefabComponent>();
    }
}
