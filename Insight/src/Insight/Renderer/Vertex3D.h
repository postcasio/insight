#pragma once

#include "VertexBufferLayout.h"
#include "Vulkan.h"
#include "../Math.h"

namespace Insight::Renderer
{
    struct Vertex3D
    {
        vec3 Position;
        vec3 Normal;
        vec3 Tangent;
        vec3 Bitangent;
        vec2 TexCoord;

        static VertexBufferLayout &GetLayout()
        {
            static Unique<VertexBufferLayout> layout = VertexBufferLayout::Create({
                { VertexElementDataType::Float3, "Position" },
                { VertexElementDataType::Float3, "Normal" },
                { VertexElementDataType::Float3, "Tangent" },
                { VertexElementDataType::Float3, "Bitangent" },
                { VertexElementDataType::Float2, "TexCoord" },
            });

            return *layout;
        }
    };
}