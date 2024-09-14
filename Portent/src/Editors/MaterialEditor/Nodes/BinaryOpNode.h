#pragma once
#include "MaterialNode.h"

namespace Portent::NodeGraph
{
    class Graph;
}


namespace Portent::Editors::MaterialEditorNodes
{
    class BinaryOpNode : public MaterialNode
    {
    public:
        explicit BinaryOpNode(NodeGraph::Graph &graph);
        ~BinaryOpNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;
    protected:
        string m_BinaryOp = "+";


        void InputAttributeConnected(NodeGraph::Attribute& inputAttribute,
            NodeGraph::Attribute& outputAttribute) override;
        void InputAttributeDisconnected(NodeGraph::Attribute& inputAttribute,
            NodeGraph::Attribute& outputAttribute) override;

    };
}
