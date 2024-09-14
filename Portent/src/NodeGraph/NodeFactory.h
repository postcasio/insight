#pragma once

#include "Graph.h"
#include "../Portent.h"


namespace Portent::NodeGraph
{
    class Node;

    class NodeFactory
    {
    public:
        NodeFactory() = default;
        virtual ~NodeFactory() = default;

        [[nodiscard]] virtual NodeGraph::Node* CreateNode(Graph& graph, const string& type) const = 0;
    };
}
