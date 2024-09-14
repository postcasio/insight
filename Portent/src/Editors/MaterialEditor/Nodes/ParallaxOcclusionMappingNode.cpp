#include "ParallaxOcclusionMappingNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    ParallaxOcclusionMappingNode::ParallaxOcclusionMappingNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Height Map",
            .Value = NodeGraph::Value(new NodeGraph::TextureRef()),
            .IsParameterizable = true
        });

        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Input UV",
            .Value = NodeGraph::Value(vec2(0.0f)),
            .IsParameterizable = true
        });


        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Output UV",
            .Value = NodeGraph::Value(vec2(0.0f)),
            .IsModifiable = false
        });
    }

    vector<string> ParallaxOcclusionMappingNode::GetIncludes()
    {
        return {"includes/ParallaxOcclusionMapping.glsl"};
    }

    string ParallaxOcclusionMappingNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto heightMapValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);
        auto inputUVValue = compiler.GetInputGlslValue(graph, m_InputAttributes[1]);

        auto outputVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format("{0} {1} = ParallaxOcclusionMapping({2}, {3}, normalize(FragPos * FragTBN));\n", NodeGraph::Value::GetGlslType(outputVariable->Type), outputVariable->Name, heightMapValue, inputUVValue);
    }
}
