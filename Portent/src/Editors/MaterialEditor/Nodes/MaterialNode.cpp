#include "MaterialNode.h"

#include "../../../NodeGraph/Graph.h"


namespace Portent::Editors::MaterialEditorNodes
{
    MaterialNode::MaterialNode(NodeGraph::Graph& graph) : Node(graph.CreateId())
    {
    }
}
