#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class SinNode final : public MaterialNode
    {
    public:
        explicit SinNode(NodeGraph::Graph &graph);
        ~SinNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.Sin", "Sin")
    };
}
