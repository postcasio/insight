#include "BoxGeometry.h"

namespace Insight
{
    vec3 project(vec3 t, vec3 n)
    {
        return (n * t) / (n * n) * n;
    }

    vec3 reject(vec3 t, vec3 n)
    {
        return t - project(t, n);
    }

    BoxGeometry::BoxGeometry(const BoxGeometryInfo& info)
    {
        BuildPlane(2, 1, 0, -1, -1, info.Depth, info.Height, info.Width, info.DepthSegments, info.HeightSegments,
                   0); // px
        BuildPlane(2, 1, 0, 1, -1, info.Depth, info.Height, -info.Width, info.DepthSegments, info.HeightSegments,
                   1); // nx
        BuildPlane(0, 2, 1, 1, 1, info.Width, info.Depth, info.Height, info.WidthSegments, info.DepthSegments, 2); // py
        BuildPlane(0, 2, 1, 1, -1, info.Width, info.Depth, -info.Height, info.WidthSegments, info.DepthSegments,
                   3); // ny
        BuildPlane(0, 1, 2, 1, -1, info.Width, info.Height, info.Depth, info.WidthSegments, info.HeightSegments,
                   4); // pz
        BuildPlane(0, 1, 2, -1, -1, info.Width, info.Height, -info.Depth, info.WidthSegments, info.HeightSegments,
                   5); // nz

        CalculateTangents();
    }

    void BoxGeometry::BuildPlane(u32 u, u32 v, u32 w, float udir, float vdir, float width, float height, float depth,
                                 u32 gridX,
                                 u32 gridY, u32 materialIndex)
    {
        const auto segmentWidth = width / static_cast<float>(gridX);
        const auto segmentHeight = height / static_cast<float>(gridY);

        const auto widthHalf = width / 2;
        const auto heightHalf = height / 2;
        const auto depthHalf = depth / 2;

        const auto gridX1 = gridX + 1;
        const auto gridY1 = gridY + 1;

        auto vertexCounter = 0;
        u32 submeshCount = 0;

        u32 submeshStart = m_Indices.size();
        u32 verticesStart = m_Vertices.size();

        for (auto iy = 0; iy < gridY1; iy++)
        {
            const auto y = iy * segmentHeight - heightHalf;

            for (auto ix = 0; ix < gridX1; ix++)
            {
                const auto x = ix * segmentWidth - widthHalf;

                vec3 position;
                position[u] = x * udir;
                position[v] = y * vdir;
                position[w] = depthHalf;
                vec3 normal;
                normal[u] = 0;
                normal[v] = 0;
                normal[w] = depth > 0 ? 1 : -1;

                vec2 uv = {
                    ix / gridX,
                     (iy / gridY)
                };

                Renderer::Vertex3D vertex = {
                    .Position = position,
                    .Normal = normal,
                    .Tangent{},
                    .Bitangent{},
                    .TexCoord = uv
                };

                m_Vertices.push_back(vertex);

                vertexCounter++;
            }
        }

        for (auto iy = 0; iy < gridY; iy++)
        {
            for (auto ix = 0; ix < gridX; ix++)
            {
                const auto a = verticesStart + ix + gridX1 * iy;
                const auto b = verticesStart + ix + gridX1 * (iy + 1);
                const auto c = verticesStart + (ix + 1) + gridX1 * (iy + 1);
                const auto d = verticesStart + (ix + 1) + gridX1 * iy;

                m_Indices.push_back(a);
                m_Indices.push_back(b);
                m_Indices.push_back(d);

                m_Indices.push_back(b);
                m_Indices.push_back(c);
                m_Indices.push_back(d);

                submeshCount += 6;
            }
        }

        m_Submeshes.push_back({0, submeshStart, submeshCount, materialIndex});

        submeshStart += submeshCount;

        verticesStart += vertexCounter;
    }
}