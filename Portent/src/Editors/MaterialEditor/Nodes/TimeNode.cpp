#include "TimeNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    TimeNode::TimeNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Time",
            .Value = NodeGraph::Value(0.0f, 0.0f, FLT_MAX),
            .IsModifiable = false
        });
    }

    vector<string> TimeNode::GetIncludes()
    {
        return {};
    }

    string TimeNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto& attribute = m_OutputAttributes[0];
        auto variable = compiler.GetAttributeVariable(attribute.GetId());

        return fmt::format(
            "{0} {1} = {2};\n",
            attribute.GetValue().GetGlslType(),
            variable->Name,
            compiler.GetOutputGlslValue(graph, attribute, "Scene.Time")
        );
    }
}
