#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::Editors::MaterialEditorCompiler::GlslGraphCompiler
{
    class GlslGraphCompiler;
}

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class SurfaceOutputNode final : public MaterialNode
    {
    public:
        explicit SurfaceOutputNode(NodeGraph::Graph &graph);
        ~SurfaceOutputNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;
        bool IsOutputNode() override { return true; }

    private:

        NODEGRAPH_NODE("Insight.Material.SurfaceOutput", "Surface Output")

    };
}
