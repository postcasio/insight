#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class UnpackNormalNode final : public MaterialNode
    {
    public:
        explicit UnpackNormalNode(NodeGraph::Graph &graph);
        ~UnpackNormalNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.UnpackNormal", "Unpack Normal")
    };
}
