#pragma once
#include "../../../NodeGraph/Node.h"

namespace Portent::Editors::MaterialEditorCompiler::GlslGraphCompiler
{
    class GlslGraphCompiler;
}

namespace Portent::NodeGraph
{
    class Graph;
}

namespace Portent::Editors::MaterialEditorNodes {

    class MaterialNode : public NodeGraph::Node
    {
    public:
        explicit MaterialNode(NodeGraph::Graph &graph);
        ~MaterialNode() override = default;

        virtual vector<string> GetIncludes() = 0;
        virtual string GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph) = 0;
        virtual bool IsOutputNode() { return false; }
    };

} // MaterialEditorNodes
// Editors
// Portent
