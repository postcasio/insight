#include "UnpackNormalNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    UnpackNormalNode::UnpackNormalNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddInputAttribute({
            .Id = graph.CreateId(),
            .Name = "Packed Normal",
            .Value = NodeGraph::Value(vec3(0.0), vec3(-1.0), vec3(1.0)),
            .IsParameterizable = true
        });


        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Normal",
            .Value = NodeGraph::Value(vec3(0.0), vec3(0.0), vec3(1.0)),
            .IsModifiable = false
        });
    }

    vector<string> UnpackNormalNode::GetIncludes()
    {
        return {"includes/UnpackNormal.glsl"};
    }

    string UnpackNormalNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto packedNormalValue = compiler.GetInputGlslValue(graph, m_InputAttributes[0]);

        auto normalVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());

        return fmt::format("{0} {1} = UnpackNormal({2});\n", NodeGraph::Value::GetGlslType(normalVariable->Type), normalVariable->Name, packedNormalValue);
    }
}
