#pragma once

#include "../../../NodeGraph/NodeFactory.h"
#include "../../../NodeGraph/Graph.h"

namespace Portent::Editors::MaterialEditorNodes {

class MaterialNodeFactory : public NodeGraph::NodeFactory {
public:
    MaterialNodeFactory();
    ~MaterialNodeFactory() override = default;

    [[nodiscard]] NodeGraph::Node* CreateNode(NodeGraph::Graph& graph, const string& type) const override;
private:
    unordered_map<string, std::function<NodeGraph::Node*(NodeGraph::Graph&)>> m_NodeConstructors;
};

}
