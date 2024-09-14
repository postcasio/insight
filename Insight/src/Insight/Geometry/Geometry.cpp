#include "Geometry.h"

namespace Insight {
    void Geometry::CalculateTangents()
    {
        for (u32 submeshIndex = 0; submeshIndex < m_Submeshes.size(); submeshIndex++)
        {
            const auto& submesh = m_Submeshes[submeshIndex];

            for (u32 startIndex = submesh.BaseIndex; startIndex < submesh.BaseIndex + submesh.IndexCount; startIndex += 3)
            {
                Renderer::Vertex3D &v0 = m_Vertices[m_Indices[startIndex]];
                Renderer::Vertex3D &v1 = m_Vertices[m_Indices[startIndex + 1]];
                Renderer::Vertex3D &v2 = m_Vertices[m_Indices[startIndex + 2]];

                vec3 &p0 = v0.Position;
                vec3 &p1 = v1.Position;
                vec3 &p2 = v2.Position;

                vec2 &w0 = v0.TexCoord;
                vec2 &w1 = v1.TexCoord;
                vec2 &w2 = v2.TexCoord;

                vec3 e1 = p1 - p0, e2 = p2 - p0;

                f32 x1 = w1.x - w0.x, x2 = w2.x - w0.x;
                f32 y1 = w1.y - w0.y, y2 = w2.y - w0.y;

                f32 r = 1.0f / (x1 * y2 - x2 * y1);

                vec3 t = glm::normalize((e1 * y2 - e2 * y1) * r);
                vec3 b = glm::normalize((e2 * x1 - e1 * x2) * r);

                for (u32 i = 0; i < 3; i++)
                {
                    const auto n = m_Vertices[m_Indices[startIndex + i]].Normal;

                    const auto tmp =  glm::normalize(t - n * glm::dot(n, t));

                    const auto tmp2 = glm::normalize(glm::cross(n, t));
                    const auto test = glm::dot(tmp2, b);
                    const auto w = test < 0.0f ? -1.0f : 1.0f;

                    m_Vertices[m_Indices[startIndex + i]].Tangent = tmp;
                    m_Vertices[m_Indices[startIndex + i]].Bitangent = glm::normalize(glm::cross(n, tmp) * w);
                }
            }
        }
    }
}