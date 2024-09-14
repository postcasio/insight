#pragma once
#include "BinaryOpNode.h"


namespace Portent::Editors::MaterialEditorNodes
{
    class SubtractNode final : public BinaryOpNode
    {
    public:
        explicit SubtractNode(NodeGraph::Graph &graph) : BinaryOpNode(graph)
        {
            m_BinaryOp = "-";
        }
    private:
        NODEGRAPH_NODE("Insight.Material.Subtract", "Subtract")
    };
}
