#include "VectorToComponentsNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    VectorToComponentsNode::VectorToComponentsNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Vector",
            .Value = NodeGraph::Value(vec4(0.0f)),
            .IsParameterizable = true,
            .AcceptsAnyNumeric = true,
        });


        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "X",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Y",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Z",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "W",
            .Value = NodeGraph::Value(0.0f),
            .IsModifiable = false
        });
    }

    vector<string> VectorToComponentsNode::GetIncludes()
    {
        return {};
    }

    string VectorToComponentsNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto vectorValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);

        string lines{};

        const char* swizzles[] = { "x", "y", "z", "w" };

        for (auto i = 0; i < 4; i++)
        {
            auto outputAttribute = m_OutputAttributes[i];
            if (outputAttribute.IsHidden()) break;
            if (!outputAttribute.IsConnected()) continue;

            auto outputVariable = compiler.GetAttributeVariable(outputAttribute.GetId())->Name;

            lines += fmt::format("{0} {1} = {2}.{3};\n", NodeGraph::Value::GetGlslType(outputAttribute.GetValue().GetType()), outputVariable, vectorValue, swizzles[i]);
        }

        return lines;
    }

    void VectorToComponentsNode::InputAttributeConnected(NodeGraph::Attribute& inputAttribute,
        NodeGraph::Attribute& outputAttribute)
    {
        auto& incomingValue = outputAttribute.GetValue();
        auto incomingComponentCount = incomingValue.GetComponentCount();
        auto incomingSingleType = incomingValue.GetComponentType();

        m_InputAttributes[0].GetValue().SetType(incomingValue.GetType());
        auto i = 0;
        for (; i < incomingComponentCount; i++)
        {
            m_OutputAttributes[i].SetHidden(false);
            m_OutputAttributes[i].GetValue().SetType(incomingSingleType);
        }
        for (; i < 4; i++)
        {
            m_OutputAttributes[i].SetHidden(true);
        }
    }

    void VectorToComponentsNode::InputAttributeTypeChanged(NodeGraph::Attribute& inputAttribute)
    {
        auto& incomingValue = inputAttribute.GetValue();
        auto incomingComponentCount = incomingValue.GetComponentCount();
        auto incomingSingleType = incomingValue.GetComponentType();

        auto i = 0;
        for (; i < incomingComponentCount; i++)
        {
            m_OutputAttributes[i].SetHidden(false);
            m_OutputAttributes[i].GetValue().SetType(incomingSingleType);
        }
        for (; i < 4; i++)
        {
            m_OutputAttributes[i].SetHidden(true);
        }
    }
}
