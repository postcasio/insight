#include "BinaryOpNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    BinaryOpNode::BinaryOpNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "A",
            .Value = NodeGraph::Value(vec4(0.0), vec4(-FLT_MAX), vec4(FLT_MAX)),
            .IsParameterizable = true,
            .AcceptsAnyNumeric = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "B",
            .Value = NodeGraph::Value(vec4(0.0), vec4(-FLT_MAX), vec4(FLT_MAX)),
            .IsParameterizable = true,
            .AcceptsAnyNumeric = true
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Output",
            .Value = NodeGraph::Value(vec4(0.0), vec4(-FLT_MAX), vec4(FLT_MAX)),
            .IsModifiable = false
        });
    }

    vector<string> BinaryOpNode::GetIncludes()
    {
        return {};
    }

    string BinaryOpNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler& compiler,
                            NodeGraph::Graph& graph)
    {
        auto inputA = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);
        auto inputB = compiler.GetInputGlslValue(graph, m_InputAttributes[1]);

        auto outputVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format("{0} {1} = {2} {3} {4};\n",
                           NodeGraph::Value::GetGlslType(outputVariable->Type),
                           outputVariable->Name,
                           inputA,
                           m_BinaryOp,
                           inputB
        );
    }

    void BinaryOpNode::InputAttributeConnected(NodeGraph::Attribute& inputAttribute,
        NodeGraph::Attribute& outputAttribute)
    {
        if (!outputAttribute.GetValue().IsSingleValue())
        {
            for (auto &attribute : m_InputAttributes)
            {
                attribute.GetValue().SetType(outputAttribute.GetValue().GetType());
            }

            m_OutputAttributes[0].GetValue().SetType(outputAttribute.GetValue().GetType());
        }
        else
        {
            inputAttribute.GetValue().SetType(outputAttribute.GetValue().GetType());

            bool allSameType = true;

            for (auto &attribute : m_InputAttributes)
            {
                if (attribute.GetValue().GetType() != outputAttribute.GetValue().GetType())
                {
                    allSameType = false;
                }
            }

            if (allSameType)
            {
                m_OutputAttributes[0].GetValue().SetType(outputAttribute.GetValue().GetType());
            }
        }
    }

    void BinaryOpNode::InputAttributeDisconnected(NodeGraph::Attribute& inputAttribute,
        NodeGraph::Attribute& outputAttribute)
    {

    }
}
