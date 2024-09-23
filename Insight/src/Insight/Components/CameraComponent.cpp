#include "../Entity.h"
#include "CameraComponent.h"

namespace Insight
{
    void CameraComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<CameraComponent>();
    }
}
