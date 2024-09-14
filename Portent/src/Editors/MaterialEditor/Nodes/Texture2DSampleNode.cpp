#include "Texture2DSampleNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    Texture2DSampleNode::Texture2DSampleNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Texture",
            .Value = NodeGraph::Value(new NodeGraph::TextureRef()),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "UV",
            .Value = NodeGraph::Value(vec2(0.0), vec2(0.0), vec2(1.0)),
            .IsParameterizable = true
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "RGBA",
            .Value = NodeGraph::Value(vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0), vec4(1.0)),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "RGB",
            .Value = NodeGraph::Value(vec3(0.0), vec3(0.0), vec3(1.0)),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "R",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "G",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "B",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsModifiable = false
        });

        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "A",
            .Value = NodeGraph::Value(1.0f, 0.0f, 1.0f),
            .IsModifiable = false
        });
    }

    vector<string> Texture2DSampleNode::GetIncludes()
    {
        return {};
    }

    string Texture2DSampleNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto textureValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);
        auto uvValue = compiler.GetInputGlslValue(graph, m_InputAttributes[1]);

        auto rgbaVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        string lines{};

        lines += fmt::format(
            "{0} {1} = texture({2}, {3});\n",
            NodeGraph::Value::GetGlslType(rgbaVariable->Type),
            rgbaVariable->Name,
            textureValue,
            uvValue
        );

        if (m_OutputAttributes[1].IsConnected())
        {
            auto rgbVariable = compiler.GetAttributeVariable(m_OutputAttributes[1].GetId());

            lines += fmt::format(
                "{0} {1} = {2}.rgb;\n",
                NodeGraph::Value::GetGlslType(rgbVariable->Type),
                rgbVariable->Name,
                rgbaVariable->Name
                );
        }

        if (m_OutputAttributes[2].IsConnected())
        {
            auto rVariable = compiler.GetAttributeVariable(m_OutputAttributes[2].GetId());

            lines += fmt::format(
                "{0} {1} = {2}.r;\n",
                NodeGraph::Value::GetGlslType(rVariable->Type),
                rVariable->Name,
                rgbaVariable->Name
            );
        }

        if (m_OutputAttributes[3].IsConnected())
        {
            auto gVariable = compiler.GetAttributeVariable(m_OutputAttributes[3].GetId());

            lines += fmt::format(
                "{0} {1} = {2}.g;\n",
                NodeGraph::Value::GetGlslType(gVariable->Type),
                gVariable->Name,
                rgbaVariable->Name
            );
        }

        if (m_OutputAttributes[4].IsConnected())
        {
            auto bVariable = compiler.GetAttributeVariable(m_OutputAttributes[4].GetId());

            lines += fmt::format(
                "{0} {1} = {2}.b;\n",
                NodeGraph::Value::GetGlslType(bVariable->Type),
                bVariable->Name,
                rgbaVariable->Name
            );
        }

        if (m_OutputAttributes[5].IsConnected())
        {
            auto aVariable = compiler.GetAttributeVariable(m_OutputAttributes[5].GetId());

            lines += fmt::format(
                "{0} {1} = {2}.a;\n",
                NodeGraph::Value::GetGlslType(aVariable->Type),
                aVariable->Name,
                rgbaVariable->Name
            );
        }

        return lines;
    }
}
