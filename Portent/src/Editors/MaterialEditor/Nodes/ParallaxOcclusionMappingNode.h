#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class ParallaxOcclusionMappingNode final : public MaterialNode
    {
    public:
        explicit ParallaxOcclusionMappingNode(NodeGraph::Graph &graph);
        ~ParallaxOcclusionMappingNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;

    private:
        NODEGRAPH_NODE("Insight.Material.ParallaxOcclusionMapping", "ParallaxOcclusionMapping")
    };
}
