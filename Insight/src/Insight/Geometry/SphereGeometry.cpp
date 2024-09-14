#include "SphereGeometry.h"

namespace Insight
{
    SphereGeometry::SphereGeometry(const SphereGeometryInfo& info)
    {
        u32 widthSegments = std::max(info.WidthSegments, 3u);
        u32 heightSegments = std::max(info.HeightSegments, 2u);

        float thetaEnd = std::min(info.ThetaStart + info.ThetaLength, glm::pi<float>());

        vector<vector<u32>> grid;
        vector<Renderer::Vertex3D> vertices;

        for (u32 iy = 0; iy <= heightSegments; iy++)
        {
            vector<u32> vertexRow;

            float v = static_cast<float>(iy) / static_cast<float>(heightSegments);

            // special case for the poles

            float uOffset = 0;

            if (iy == 0 && info.ThetaStart == 0)
            {
                uOffset = 0.5f / static_cast<float>(widthSegments);
            }
            else if (iy == heightSegments && thetaEnd >= glm::pi<float>())
            {
                uOffset = -0.5f / static_cast<float>(widthSegments);
            }

            for (u32 ix = 0; ix <= widthSegments; ix++)
            {
                float u = static_cast<float>(ix) / static_cast<float>(widthSegments);

                Renderer::Vertex3D vertex{};

                vertex.Position.x = -info.Radius * glm::cos(info.PhiStart + u * info.PhiLength) * glm::sin(info.ThetaStart + v * info.ThetaLength);
                vertex.Position.y = info.Radius * glm::cos(info.ThetaStart + v * info.ThetaLength);
                vertex.Position.z = info.Radius * glm::sin(info.PhiStart + u * info.PhiLength) * glm::sin(info.ThetaStart + v * info.ThetaLength);

                vertex.Normal = glm::normalize(vertex.Position);

                vertex.TexCoord = {u + uOffset, 1 - v};

                vertexRow.push_back(static_cast<u32>(vertices.size()));
                vertices.push_back(vertex);
            }

            grid.push_back(vertexRow);
        }

        for (u32 iy = 0; iy < heightSegments; iy++)
        {
            for (u32 ix = 0; ix < widthSegments; ix++)
            {
                u32 a = grid[iy][ix + 1];
                u32 b = grid[iy][ix];
                u32 c = grid[iy + 1][ix];
                u32 d = grid[iy + 1][ix + 1];

                if (iy != 0 || info.ThetaStart > 0)
                {
                    m_Indices.push_back(a);
                    m_Indices.push_back(b);
                    m_Indices.push_back(d);
                }

                if (iy != heightSegments - 1 || thetaEnd < glm::pi<float>())
                {
                    m_Indices.push_back(b);
                    m_Indices.push_back(c);
                    m_Indices.push_back(d);
                }
            }
        }

        m_Vertices = vertices;
        m_Submeshes.push_back({0, 0, static_cast<u32>(m_Indices.size()), 0});

        CalculateTangents();
    }
}
