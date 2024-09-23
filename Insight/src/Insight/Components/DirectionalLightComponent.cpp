#include "../Entity.h"
#include "DirectionalLightComponent.h"

namespace Insight
{
    void DirectionalLightComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<DirectionalLightComponent>();
    }
}
