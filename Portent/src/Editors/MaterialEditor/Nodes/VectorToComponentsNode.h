#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class VectorToComponentsNode final : public MaterialNode
    {
    public:
        explicit VectorToComponentsNode(NodeGraph::Graph &graph);
        ~VectorToComponentsNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;
    private:
        NODEGRAPH_NODE("Insight.Material.VectorToComponents", "Vector to Components")

    protected:
        void InputAttributeConnected(NodeGraph::Attribute& inputAttribute,
            NodeGraph::Attribute& outputAttribute) override;
        void InputAttributeTypeChanged(NodeGraph::Attribute& inputAttribute) override;
    };
}
