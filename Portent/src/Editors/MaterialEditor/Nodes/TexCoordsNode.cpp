#include "TexCoordsNode.h"
#include "../../../NodeGraph/Graph.h"
#include "../Compiler/GlslGraphCompiler.h"


namespace Portent::Editors::MaterialEditorNodes
{
    TexCoordsNode::TexCoordsNode(NodeGraph::Graph& graph) : MaterialNode(graph)
    {
        AddOutputAttribute({
            .Id = graph.CreateId(),
            .Name = "UV",
            .Value = NodeGraph::Value(vec2(0.0f), vec2(0.0f), vec2(1.0f)),
            .IsModifiable = false
        });
    }

    vector<string> TexCoordsNode::GetIncludes()
    {
        return {};
    }

    string TexCoordsNode::GetCode(MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler &compiler, NodeGraph::Graph& graph)
    {
        auto& attribute = m_OutputAttributes[0];
        auto variable = compiler.GetAttributeVariable(attribute.GetId());

        return fmt::format(
            "{0} {1} = {2};\n",
            attribute.GetValue().GetGlslType(),
            variable->Name,
            compiler.GetOutputGlslValue(graph, attribute, "FragTexCoord")
        );
    }
}
