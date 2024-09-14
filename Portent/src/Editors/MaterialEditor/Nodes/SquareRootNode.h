#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class SquareRootNode final : public MaterialNode
    {
    public:
        explicit SquareRootNode(NodeGraph::Graph &graph);
        ~SquareRootNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.SquareRoot", "Square Root")
    };
}
