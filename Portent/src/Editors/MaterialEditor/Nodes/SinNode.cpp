#include "SinNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    SinNode::SinNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Input",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true
        });


        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Output",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false
        });
    }

    vector<string> SinNode::GetIncludes()
    {
        return {};
    }

    string SinNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto packedNormalValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);

        auto normalVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format("{0} {1} = sin({2});\n", NodeGraph::Value::GetGlslType(normalVariable->Type), normalVariable->Name, packedNormalValue);
    }
}
