#pragma once

#include "../Portent.h"
#include "Node.h"


namespace Portent::NodeGraph
{
    class NodeFactory;

    class Graph
    {
    public:
        ~Graph();

        Id CreateId()
        {
            return ++m_IdCounter;
        }

        void AddNode(Node* node)
        {
            m_Nodes.push_back(node);
        }

        void RemoveNode(Node* node)
        {
            for (auto i = 0; i < m_InputLinks.size(); i++)
            {
                if (m_InputLinks[i].SourceNode == node->GetId() || m_InputLinks[i].DestinationNode == node->GetId())
                {
                    RemoveLink(i);
                    i--;
                }
            }

            std::erase(m_Nodes, node);

            node->Dispose();

            delete node;
        }

        void RemoveNode(const Id id)
        {
            auto node = GetNode(id);

            if (node)
            {
                RemoveNode(node);
            }
        }

        [[nodiscard]] vector<Node*> GetNodes() const
        {
            return m_Nodes;
        }

        [[nodiscard]] vector<Link> GetInputLinks() const
        {
            return m_InputLinks;
        }

        [[nodiscard]] vector<Link> GetInputLinks(Id attributeId) const;
        bool TryGetInputLink(Id attributeId, Link& outLink) const;

        void AddLink(Id first, Id second);
        void ValidateLinks();

        [[nodiscard]] bool IsInputAttribute(Id attribute) const;
        [[nodiscard]] Node* GetNode(Id id) const;
        template <Derived<Node> T>
        [[nodiscard]] T* GetNode(const Id id) const
        {
            return dynamic_cast<T*>(GetNode(id));
        }
        [[nodiscard]] Node* GetNodeContainingAttribute(Id id) const;
        [[nodiscard]] bool IsInputAttributeConnected(Id inputAttributeId) const;
        [[nodiscard]] bool IsOutputAttributeConnected(Id outputAttributeId) const;
        void RemoveLink(i32 inputLinkIndex);
        json Serialize();
        void Deserialize(json::const_reference value, const NodeFactory& factory);
        Value DeserializeValue(ValueType type, json::const_reference data, json::const_reference minData, json::const_reference maxData);

        static bool AreAttributesCompatible(const Attribute& input, const Attribute& output);

        [[nodiscard]] vector<Attribute*> GetParameterizedAttributes() const;

    private:
        Id m_IdCounter = 0;
        vector<Node *> m_Nodes{};
        vector<Link> m_InputLinks{};
    };
} // Portent
