#include "MaterialNodeFactory.h"

#include "AddNode.h"
#include "ComponentsToVectorNode.h"
#include "DotProductNode.h"
#include "ModuloNode.h"
#include "MultiplyNode.h"
#include "ParallaxOcclusionMappingNode.h"
#include "SinNode.h"
#include "SquareRootNode.h"
#include "SubtractNode.h"
#include "SurfaceOutputNode.h"
#include "TexCoordsNode.h"
#include "Texture2DNode.h"
#include "Texture2DSampleNode.h"
#include "TimeNode.h"
#include "UnpackNormalNode.h"
#include "VectorToComponentsNode.h"


namespace Portent::Editors::MaterialEditorNodes {
    MaterialNodeFactory::MaterialNodeFactory()
    {
        m_NodeConstructors = {
            {"Insight.Material.Add", [](NodeGraph::Graph& graph) { return new AddNode(graph); }},
            {"Insight.Material.Subtract", [](NodeGraph::Graph& graph) { return new SubtractNode(graph); }},
            {"Insight.Material.Multiply", [](NodeGraph::Graph& graph) { return new MultiplyNode(graph); }},
            {"Insight.Material.Modulo", [](NodeGraph::Graph& graph) { return new ModuloNode(graph); }},
            {"Insight.Material.Sin", [](NodeGraph::Graph& graph) { return new SinNode(graph); }},
            {"Insight.Material.DotProduct", [](NodeGraph::Graph& graph) { return new DotProductNode(graph); }},
            {"Insight.Material.SquareRoot", [](NodeGraph::Graph& graph) { return new SquareRootNode(graph); }},
            {"Insight.Material.Time", [](NodeGraph::Graph& graph) { return new TimeNode(graph); }},
            {"Insight.Material.ParallaxOcclusionMapping", [](NodeGraph::Graph& graph) { return new ParallaxOcclusionMappingNode(graph); }},
            {"Insight.Material.UnpackNormal", [](NodeGraph::Graph& graph) { return new UnpackNormalNode(graph); }},
            {"Insight.Material.Texture2D", [](NodeGraph::Graph& graph) { return new Texture2DNode(graph); }},
            {"Insight.Material.Texture2DSample", [](NodeGraph::Graph& graph) { return new Texture2DSampleNode(graph); }},
            {"Insight.Material.TexCoords", [](NodeGraph::Graph& graph) { return new TexCoordsNode(graph); }},
            {"Insight.Material.VectorToComponents", [](NodeGraph::Graph& graph) { return new VectorToComponentsNode(graph); }},
            {"Insight.Material.ComponentsToVector", [](NodeGraph::Graph& graph) { return new ComponentsToVectorNode(graph); }},
            {"Insight.Material.SurfaceOutput", [](NodeGraph::Graph& graph) { return new SurfaceOutputNode(graph); }},
        };
    }

    NodeGraph::Node* MaterialNodeFactory::CreateNode(NodeGraph::Graph& graph, const string& type) const
    {
        auto it = m_NodeConstructors.find(type);
        if (it != m_NodeConstructors.end())
        {
            return it->second(graph);
        }

        return nullptr;
    }
}
