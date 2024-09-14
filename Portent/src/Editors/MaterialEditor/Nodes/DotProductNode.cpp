#include "DotProductNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    DotProductNode::DotProductNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "A",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true,
            .AcceptsAnyNumeric = true,
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "B",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true,
            .AcceptsAnyNumeric = true,
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Result",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false,
            .IsParameterizable = false,
        });
    }

    vector<string> DotProductNode::GetIncludes()
    {
        return {};
    }

    string DotProductNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto aValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);
        auto bValue = compiler.GetInputGlslValue(graph, m_InputAttributes[1]);

        auto outputVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format(
            "{0} {1} = dot({2}, {3});\n",
            NodeGraph::Value::GetGlslType(outputVariable->Type),
            outputVariable->Name,
            aValue,
            bValue
        );
    }
}
