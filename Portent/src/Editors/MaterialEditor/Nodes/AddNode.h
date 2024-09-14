#pragma once
#include "BinaryOpNode.h"


namespace Portent::Editors::MaterialEditorNodes
{
    class AddNode final : public BinaryOpNode
    {
    public:
        explicit AddNode(NodeGraph::Graph &graph) : BinaryOpNode(graph)
        {
            m_BinaryOp = "+";
        }
    private:
        NODEGRAPH_NODE("Insight.Material.Add", "Add")
    };
}
