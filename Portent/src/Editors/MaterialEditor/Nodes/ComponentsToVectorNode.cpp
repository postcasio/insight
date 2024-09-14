#include "ComponentsToVectorNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    ComponentsToVectorNode::ComponentsToVectorNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "X",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Y",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Z",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "W",
            .Value = NodeGraph::Value(0.0f),
            .IsParameterizable = true
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Vector",
            .Value = NodeGraph::Value(vec4(0.0f)),
            .IsModifiable = false,
            .IsTypeModifiable = true,
            .AcceptsAnyNumeric = true,
        });
    }

    vector<string> ComponentsToVectorNode::GetIncludes()
    {
        return {};
    }

    string ComponentsToVectorNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler& compiler,
                                           NodeGraph::Graph& graph)
    {
        string components;

        switch (m_OutputAttributes[0].GetValue().GetComponentCount())
        {
        default:
        case 1:
            components = fmt::format(
                "{0}({1})", NodeGraph::Value::GetGlslType(m_OutputAttributes[0].GetValue().GetType()),
                compiler.GetInputGlslValue(graph, m_InputAttributes[0]));
            break;
        case 2:
            components = fmt::format(
                "{0}({1}, {2})", NodeGraph::Value::GetGlslType(m_OutputAttributes[0].GetValue().GetType()),
                compiler.GetInputGlslValue(graph, m_InputAttributes[0]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[1]));
            break;
        case 3:
            components = fmt::format(
                "{0}({1}, {2}, {3})", NodeGraph::Value::GetGlslType(m_OutputAttributes[0].GetValue().GetType()),
                compiler.GetInputGlslValue(graph, m_InputAttributes[0]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[1]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[2]));
            break;
        case 4:
            components = fmt::format(
                "{0}({1}, {2}, {3}, {4})", NodeGraph::Value::GetGlslType(m_OutputAttributes[0].GetValue().GetType()),
                compiler.GetInputGlslValue(graph, m_InputAttributes[0]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[1]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[2]),
                compiler.GetInputGlslValue(graph, m_InputAttributes[3]));
            break;
        }

        return fmt::format("{0} {1} = {2};\n",
                           NodeGraph::Value::GetGlslType(m_OutputAttributes[0].GetValue().GetType()),
                           compiler.GetAttributeVariable(m_OutputAttributes[0].GetId())->Name, components);
    }

    void ComponentsToVectorNode::OutputAttributeTypeChanged(NodeGraph::Attribute& outputAttribute)
    {
        const auto componentCount = outputAttribute.GetValue().GetComponentCount();

        for (auto i = 0; i < componentCount; i++)
        {
            m_InputAttributes[i].SetHidden(false);
        }

        for (auto i = componentCount; i < 4; i++)
        {
            m_InputAttributes[i].SetHidden(true);
        }
    }
}
