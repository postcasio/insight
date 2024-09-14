#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes {

    class TimeNode final : public MaterialNode
    {
    public:
        explicit TimeNode(NodeGraph::Graph &graph);
        ~TimeNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.Time", "Time")
    };

} // MaterialEditorNodes
// Editors
// Portent
