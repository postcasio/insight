#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes {

    class TexCoordsNode final : public MaterialNode
    {
    public:
        explicit TexCoordsNode(NodeGraph::Graph &graph);
        ~TexCoordsNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.TexCoords", "TexCoords")
    };

} // MaterialEditorNodes
// Editors
// Portent
