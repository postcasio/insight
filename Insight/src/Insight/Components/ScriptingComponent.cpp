#include "../Entity.h"
#include "ScriptingComponent.h"

namespace Insight
{
    void ScriptingComponent::AddFunction(Entity* entity)
    {
        entity->AddComponent<ScriptingComponent>();
    }
}
