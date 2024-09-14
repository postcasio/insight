#include "PlaneGeometry.h"

namespace Insight {
    PlaneGeometry::PlaneGeometry(const PlaneGeometryInfo& info)
    {
        const auto widthHalf = info.Width / 2;
        const auto heightHalf = info.Height / 2;

        const auto gridX = info.WidthSegments;
        const auto gridY = info.HeightSegments;

        const auto gridX1 = gridX + 1;
        const auto gridY1 = gridY + 1;

        const auto segmentWidth = info.Width / static_cast<float>(gridX);
        const auto segmentHeight = info.Height / static_cast<float>(gridY);

        for (auto iy = 0; iy < gridY1; iy++)
        {
            const auto y = static_cast<float>(iy) * segmentHeight - heightHalf;

            for (auto ix = 0; ix < gridX1; ix++)
            {
                const auto x = static_cast<float>(ix) * segmentWidth - widthHalf;

                const Renderer::Vertex3D vertex = {
                    .Position = {x, -y, 0},
                    .Normal = {0, 0, 1},
                    .TexCoord = info.UvScale * vec2(static_cast<float>(ix) / static_cast<float>(gridX), 1 - (static_cast<float>(iy) / static_cast<float>(gridY))),
                };

                m_Vertices.push_back(vertex);
            }
        }

        for (auto iy = 0; iy < gridY; iy++)
        {
            for (auto ix = 0; ix < gridX; ix++)
            {
                const auto a = ix + gridX1 * iy;
                const auto b = ix + gridX1 * (iy + 1);
                const auto c = (ix + 1) + gridX1 * (iy + 1);
                const auto d = (ix + 1) + gridX1 * iy;

                m_Indices.push_back(a);
                m_Indices.push_back(b);
                m_Indices.push_back(d);
                m_Indices.push_back(b);
                m_Indices.push_back(c);
                m_Indices.push_back(d);
            }
        }

        m_Submeshes.push_back({
            .BaseVertex = 0,
            .BaseIndex = 0,
            .IndexCount = static_cast<u32>(m_Indices.size()),
        });

        CalculateTangents();
    }
}