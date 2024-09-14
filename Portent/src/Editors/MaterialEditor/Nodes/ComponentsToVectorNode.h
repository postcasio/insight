#pragma once
#include "MaterialNode.h"
#include "../../../NodeGraph/Node.h"

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes
{
    class ComponentsToVectorNode final : public MaterialNode
    {
    public:
        explicit ComponentsToVectorNode(NodeGraph::Graph &graph);
        ~ComponentsToVectorNode() override = default;

        vector<string> GetIncludes() override;
        string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) override;
    private:
        NODEGRAPH_NODE("Insight.Material.ComponentsToVector", "Components to Vector")

    protected:
        void OutputAttributeTypeChanged(NodeGraph::Attribute& outputAttribute) override;
    };
}
