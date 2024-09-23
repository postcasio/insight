#include "Entity.h"
#include "Scene.h"
#include "Application.h"


Insight::Entity::Entity(const EntityHandle handle, Scene* scene): m_Handle(handle), m_Scene(scene)
{}

std::size_t std::hash<Insight::Entity>::operator()(Insight::Entity const& s) const noexcept
{
    return static_cast<std::size_t>(s.GetHandle());
}
