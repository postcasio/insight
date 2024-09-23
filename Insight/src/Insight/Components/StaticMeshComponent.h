#pragma once

#include "../Insight.h"
#include "../Assets/StaticMesh.h"

namespace Insight
{
    class Entity;

    struct StaticMeshComponent
    {
        static inline const Uuid ComponentId = Uuid {"d1bc1167-cf94-4f9e-a6b7-8f8221204501"};
        static inline const string ComponentName = "StaticMeshComponent";
        static void AddFunction(Entity* entity);

        Ref<StaticMesh> Mesh;

        StaticMeshComponent() = default;
        StaticMeshComponent(const StaticMeshComponent&) = default;

        explicit StaticMeshComponent(const Ref<StaticMesh>& mesh) : Mesh(mesh)
        {
        }
    };
}
