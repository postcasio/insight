#pragma once

#include "../Insight.h"
#include "../Assets/StaticMesh.h"

namespace Insight
{
    struct StaticMeshComponent
    {
        Ref<StaticMesh> Mesh;

        StaticMeshComponent() = default;
        StaticMeshComponent(const StaticMeshComponent&) = default;

        explicit StaticMeshComponent(const Ref<StaticMesh>& mesh) : Mesh(mesh)
        {
        }
    };
}
