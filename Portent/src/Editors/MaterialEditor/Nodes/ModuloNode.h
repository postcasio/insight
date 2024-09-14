#pragma once
#include "BinaryOpNode.h"


namespace Portent::Editors::MaterialEditorNodes
{
    class ModuloNode final : public BinaryOpNode
    {
    public:
        explicit ModuloNode(NodeGraph::Graph &graph) : BinaryOpNode(graph)
        {
            m_BinaryOp = "%";
        }
    private:
        NODEGRAPH_NODE("Insight.Material.Modulo", "Modulo")
    };
}
