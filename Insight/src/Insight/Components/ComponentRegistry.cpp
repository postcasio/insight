#include "ComponentRegistry.h"

#include "CameraComponent.h"
#include "DirectionalLightComponent.h"
#include "EnvironmentComponent.h"
#include "HierarchyComponent.h"
#include "NameComponent.h"
#include "PointLightComponent.h"
#include "PrefabComponent.h"
#include "StaticMeshComponent.h"
#include "TransformComponent.h"
#include "UuidComponent.h"

#include "../Scripting/Components/TransformComponent.h"
#include "../Scripting/Components/StaticMeshComponent.h"

namespace Insight
{
#define REGISTER_COMPONENT(component, jsclass) {\
    component::ComponentId,\
    component::ComponentName,\
    component::AddFunction,\
    jsclass\
}

    ComponentRegistry::ComponentRegistry()
    {
        m_Registry.push_back(REGISTER_COMPONENT(CameraComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(DirectionalLightComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(EnvironmentComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(HierarchyComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(NameComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(PointLightComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(PrefabComponent, nullptr));
        m_Registry.push_back(REGISTER_COMPONENT(StaticMeshComponent, &Scripting::ScriptStaticMeshComponent::Clasp));
        m_Registry.push_back(REGISTER_COMPONENT(TransformComponent, &Scripting::ScriptTransformComponent::Clasp));
        m_Registry.push_back(REGISTER_COMPONENT(UuidComponent, nullptr));
    }

    const ComponentRegistryEntry& ComponentRegistry::GetEntry(Uuid componentId) const
    {
        for (const auto& entry : m_Registry)
        {
            if (entry.ComponentId == componentId)
            {
                return entry;
            }
        }

        throw std::runtime_error("Component not found in registry");
    }

    const ComponentRegistryEntry& ComponentRegistry::GetEntry(const i32 index) const
    {
        return m_Registry[index];
    }

    const i32 ComponentRegistry::GetIndex(Uuid uuid)
    {
        for (i32 i = 0; i < m_Registry.size(); i++)
        {
            if (m_Registry[i].ComponentId == uuid)
            {
                return i;
            }
        }

        return -1;
    }
}
