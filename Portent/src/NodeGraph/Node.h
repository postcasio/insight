#pragma once

#include "Attribute.h"
#include "GraphTypes.h"
#include "Insight/Uuid.h"

#define NODEGRAPH_NODE(type, name) \
    private: \
        static inline const string s_Name = name; \
        static inline const string s_NodeType{type}; \
    public: \
        [[nodiscard]] string GetName() const override { return s_Name; } \
        [[nodiscard]] string GetNodeType() const override { return s_NodeType; }

namespace Portent::NodeGraph
{
    class Node
    {
    public:
        explicit Node(const Id id) : m_Id(id)
        {
        };
        virtual ~Node() = default;

        [[nodiscard]] Id GetId() const { return m_Id; }
        void SetId(const Id& id) { m_Id = id; }
        [[nodiscard]] vector<Attribute>& GetInputAttributes() { return m_InputAttributes; }
        [[nodiscard]] vector<Attribute>& GetOutputAttributes() { return m_OutputAttributes; }

        [[nodiscard]] virtual string GetNodeType() const = 0;
        [[nodiscard]] virtual string GetName() const = 0;

        void Dispose()
        {
            for (auto& attribute : m_InputAttributes)
            {
                auto& value = attribute.GetValue();
                switch (value.GetType())
                {
                case ValueType::Texture2D:
                    delete value.GetData().Texture2D;
                    break;
                default:
                    break;
                }
            }

            for (auto& attribute : m_OutputAttributes)
            {
                auto& value = attribute.GetValue();
                switch (value.GetType())
                {
                case ValueType::Texture2D:
                    delete value.GetData().Texture2D;
                    break;
                default:
                    break;
                }
            }
        }

        [[nodiscard]] Attribute& GetInputAttribute(const Id id)
        {
            for (auto& attribute : m_InputAttributes)
            {
                if (attribute.GetId() == id)
                {
                    return attribute;
                }
            }

            throw std::runtime_error("Attribute not found");
        }

        [[nodiscard]] Attribute& GetInputAttributeAt(const i32 index)
        {
            return m_InputAttributes.at(index);
        }

        [[nodiscard]] Attribute& GetOutputAttributeAt(const i32 index)
        {
            return m_OutputAttributes.at(index);
        }

        [[nodiscard]] Attribute& GetOutputAttribute(const Id id)
        {
            for (auto& attribute : m_OutputAttributes)
            {
                if (attribute.GetId() == id)
                {
                    return attribute;
                }
            }

            throw std::runtime_error("Attribute not found");
        }

        [[nodiscard]] vec2 GetPosition() const { return m_Position; }
        void SetPosition(const vec2& position) { m_Position = position; m_ShouldUpdatePosition = true; }
        bool ShouldUpdatePosition() const { return m_ShouldUpdatePosition; }

    protected:
        void AddInputAttribute(const AttributeInfo& info)
        {
            m_InputAttributes.emplace_back(info);
        }

        void AddOutputAttribute(const AttributeInfo& info)
        {
            m_OutputAttributes.emplace_back(info);
        }

        virtual void InputAttributeConnected(Attribute& inputAttribute, Attribute& outputAttribute) {}
        virtual void InputAttributeDisconnected(Attribute& inputAttribute, Attribute& outputAttribute) {}
        virtual void OutputAttributeConnected(Attribute& outputAttribute, Attribute& inputAttribute) {}
        virtual void OutputAttributeDisconnected(Attribute& outputAttribute, Attribute& inputAttribute) {}
        virtual void InputAttributeTypeChanged(Attribute& inputAttribute) {}
        virtual void OutputAttributeTypeChanged(Attribute& outputAttribute) {}

        Id m_Id;

        vector<Attribute> m_InputAttributes;
        vector<Attribute> m_OutputAttributes;

        vec2 m_Position = {0, 0};
        bool m_ShouldUpdatePosition = false;

        void ShouldUpdatePosition(bool shouldUpdate) { m_ShouldUpdatePosition = shouldUpdate; }

        friend class Graph;
        friend class Renderer;
    };
}
