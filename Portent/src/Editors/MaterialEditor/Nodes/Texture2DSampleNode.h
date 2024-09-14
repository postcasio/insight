#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class Texture2DSampleNode final : public MaterialNode
    {
    public:
        explicit Texture2DSampleNode(NodeGraph::Graph &graph);
        ~Texture2DSampleNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.Texture2DSample", "Texture2D Sample")
    };
}
