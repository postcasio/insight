#pragma once

#include "../Insight.h"
#include "Vulkan.h"

namespace Insight::Renderer {

    enum class VertexElementDataType
    {
        None = 0,
        Float,
        Float2,
        Float3
    };

    struct VertexBufferElementDefinition
    {
        VertexElementDataType Type;
        const char *Name;
    };

    struct VertexBufferElement
    {
        VertexElementDataType Type;
        std::string Name;
        uint32_t Size;
        uint32_t Offset;

        VertexBufferElement(VertexElementDataType type, const std::string &name);
    };

    vk::Format VertexElementDataTypeToVulkanFormat(VertexElementDataType type);

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout(const std::initializer_list<VertexBufferElement> &elements);

        [[nodiscard]] const std::vector<VertexBufferElement> &GetElements() const
        {
            return m_Elements;
        }
        [[nodiscard]] u32 GetStride() const
        {
            return m_Stride;
        }

        static Unique<VertexBufferLayout> Create(const std::initializer_list<VertexBufferElement> &elements)
        {
            return CreateUnique<VertexBufferLayout>(elements);
        }

    private:
        std::vector<VertexBufferElement> m_Elements = {};
        uint32_t m_Stride = 0;
    };

}
