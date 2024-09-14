#pragma once
#include "BinaryOpNode.h"


namespace Portent::Editors::MaterialEditorNodes
{
    class MultiplyNode final : public BinaryOpNode
    {
    public:
        explicit MultiplyNode(NodeGraph::Graph &graph) : BinaryOpNode(graph)
        {
            m_BinaryOp = "*";
        }
    private:
        NODEGRAPH_NODE("Insight.Material.Multiply", "Multiply")
    };
}
