#include "SquareRootNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    SquareRootNode::SquareRootNode(NodeGraph::Graph& graph) : MaterialNode(graph)
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

    vector<string> SquareRootNode::GetIncludes()
    {
        return {};
    }

    string SquareRootNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto inputValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);

        auto outputVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format("{0} {1} = sqrt({2});\n", NodeGraph::Value::GetGlslType(outputVariable->Type), outputVariable->Name, inputValue);
    }
}
