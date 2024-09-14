#pragma once
#include "../Insight.h"
#include "../Assets/StaticMesh.h"
#include "../Renderer/Vertex3D.h"

namespace Insight {

class Geometry {
public:
    virtual ~Geometry() = default;

    [[nodiscard]] vector<Renderer::Vertex3D>& GetVertices() { return m_Vertices; }
    [[nodiscard]] vector<u32>& GetIndices()  { return m_Indices; }
    [[nodiscard]] vector<StaticMeshSubmesh>& GetSubmeshes() { return m_Submeshes; }

    void CalculateTangents();

protected:
    vector<Renderer::Vertex3D> m_Vertices {};
    vector<u32> m_Indices {};
    vector<StaticMeshSubmesh> m_Submeshes {};
};

} // Insight
