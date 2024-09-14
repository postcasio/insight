#pragma once

#include <queue>

#include "Insight/Ui/ImGui.h"
#include "Graph.h"



namespace Portent::NodeGraph
{
    class Renderer
    {
    public:
        explicit Renderer(Graph &graph)
            : m_Graph(graph)
        {
        }

        void Render();
        void End();
        void SetNodePosition(Node* node, const ImVec2& position);
        static void RenderParameter(Attribute* attribute);

    private:
        struct NodeRenderState
        {
            ImVec2 LargestAttributeNameSize = { 0.0f, 0.0f };
            ImVec2 Position;
            bool PositionNeedsUpdate = false;
        };

        static void RenderNodeAttributeValue(Value& value);
        void RenderNodeContents(Node *node);
        void UpdateNodeRenderState(Node* node);
        void RenderNode(Node* node);
        void RenderLinks();

        Graph &m_Graph;
        unordered_map<Id, NodeRenderState> m_RenderStates;
        std::queue<Id> m_NodeDeleteQueue;
    };
}
