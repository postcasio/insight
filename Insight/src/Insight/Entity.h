#pragma once

#include <entt/entt.hpp>
#include "Scene.h"
#include "Assert.h"

namespace Insight
{
    using EntityHandle = entt::entity;

    class Entity
    {
    public:
        Entity() = default;
        Entity(const EntityHandle handle, Scene* scene);

        template <typename T, typename... Args>
        T &AddComponent(Args &&...args) const
        {
            INS_ENGINE_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T &component = m_Scene->GetRegistry().emplace<T>(m_Handle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);

            return component;
        }
        template <typename T, typename... Args>
        T &AddOrReplaceComponent(Args &&...args) const
        {
            T &component = m_Scene->GetRegistry().emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);

            return component;
        }

        template <typename T>
        T &GetComponent() const
        {
            INS_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->GetRegistry().get<T>(m_Handle);
        }

        template <typename T>
        bool HasComponent() const
        {
            return m_Scene->GetRegistry().all_of<T>(m_Handle);
        }

        template <typename T>
        void RemoveComponent() const
        {
            INS_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->GetRegistry().remove<T>(m_Handle);
        }

        bool operator==(const Entity &other) const
        {
            return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
        }

        bool operator!=(const Entity &other) const
        {
            return !(*this == other);
        }

        [[nodiscard]] EntityHandle GetHandle() const { return m_Handle; }
        [[nodiscard]] bool IsValid() const { return m_Handle != entt::null && m_Scene != nullptr;}

        static constexpr EntityHandle NullHandle {entt::null};

    private:
        EntityHandle m_Handle = entt::null;
        Scene *m_Scene = nullptr;
    };
}

template <>
struct std::hash<Insight::Entity>
{
    std::size_t operator()(Insight::Entity const& s) const noexcept;
};
