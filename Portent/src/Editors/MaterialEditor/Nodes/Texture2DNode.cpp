#include "Texture2DNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    Texture2DNode::Texture2DNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "Texture",
            .Value = NodeGraph::Value(new NodeGraph::TextureRef()),
            .IsParameterizable = true,
            .IsModifiable = true
        });
    }

    vector<string> Texture2DNode::GetIncludes()
    {
        return {};
    }

    string Texture2DNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        return "";
        // auto textureVariable = compiler.GetAttributeVariable(m_OutputAttributes[0].GetId());
        // auto textureValue = compiler.GetOutputGlslValue(graph, m_InputAttributes[0], "Texture2D(0)");
        //
        // return fmt::format(
        //     "{0} {1} = {2};\n",
        //     NodeGraph::Value::GetGlslType(textureVariable->Type),
        //     textureVariable->Name,
        //     textureValue
        // );
    }
}
