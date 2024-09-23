#include "../Entity.h"
#include "PointLightComponent.h"

namespace Insight
{
    void PointLightComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<PointLightComponent>();
    }
}
