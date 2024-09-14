#include "SurfaceOutputNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"

namespace Portent::Editors::MaterialEditorNodes
{
    using namespace MaterialEditorCompiler::GlslGraphCompiler;

    SurfaceOutputNode::SurfaceOutputNode(NodeGraph::Graph& graph) : MaterialNode(
        graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Albedo",
            .Value = NodeGraph::Value(vec3(0.0), vec3(0.0), vec3(1.0)),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Normal",
            .Value = NodeGraph::Value(vec3(0.0), vec3(-1.0), vec3(1.0)),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Metallic",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Roughness",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Ambient Occlusion",
            .Value = NodeGraph::Value(0.0f, 0.0f, 1.0f),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Emission",
            .Value = NodeGraph::Value(vec3(0.0), vec3(0.0), vec3(1.0)),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Alpha",
            .Value = NodeGraph::Value(1.0f),
            .IsParameterizable = true
        });
    }

    vector<string> SurfaceOutputNode::GetIncludes()
    {
        return {
            "includes/Scene.glsl",
            "includes/GBufferFragIo.glsl",
        };
    }

    string SurfaceOutputNode::GetCode(GlslGraphCompiler& compiler, NodeGraph::Graph& graph)
    {
        string albedoValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);
        string normalValue = compiler.GetInputGlslValue(graph, m_InputAttributes[1]);
        string metallicValue = compiler.GetInputGlslValue(graph, m_InputAttributes[2]);
        string roughnessValue = compiler.GetInputGlslValue(graph, m_InputAttributes[3]);
        string ambientOcclusionValue = compiler.GetInputGlslValue(graph, m_InputAttributes[4]);
        string emissionValue = compiler.GetInputGlslValue(graph, m_InputAttributes[5]);
        string alphaValue = compiler.GetInputGlslValue(graph, m_InputAttributes[6]);

        return fmt::format(
            "OutPosition = vec4(FragPos, 1.0);\n"
            "OutAlbedo = vec4(vec3({0}) + vec3({6}), {1});\n"
            "OutNormal = vec4(vec3({2}), 1.0);\n"
            "OutOrm = vec4({3}, {4}, {5}, 1.0);\n",
            albedoValue,
            alphaValue,
            normalValue,
            ambientOcclusionValue,
            roughnessValue,
            metallicValue,
            emissionValue
        );
    }
}
