#include "Graph.h"

#include "NodeFactory.h"
#include "Insight/Ui/ImNodes.h"

namespace Portent::NodeGraph
{
    Graph::~Graph()
    {
        for (auto& node : m_Nodes)
        {
            node->Dispose();

            delete node;
        }
    }

    vector<Link> Graph::GetInputLinks(const Id attributeId) const
    {
        vector<Link> links{};

        for (const auto& link : m_InputLinks)
        {
            if (link.SourceAttribute == attributeId)
            {
                links.push_back(link);
            }
        }

        return links;
    }

    bool Graph::TryGetInputLink(const Id attributeId, Link& outLink) const
    {
        auto links = GetInputLinks(attributeId);

        if (!links.empty())
        {
            outLink = links[0];
            return true;
        }

        return false;
    }

    void Graph::AddLink(const Id first, const Id second)
    {
        // figure out which is the input and which is the output.
        Id input;
        Id output;

        Node* inputNode = nullptr;
        Node* outputNode = nullptr;

        if (IsInputAttribute(first))
        {
            inputNode = GetNodeContainingAttribute(first);
            outputNode = GetNodeContainingAttribute(second);
            input = first;
            output = second;
        }
        else
        {
            inputNode = GetNodeContainingAttribute(second);
            outputNode = GetNodeContainingAttribute(first);
            input = second;
            output = first;
        }

        auto& inputAttribute = inputNode->GetInputAttribute(input);
        auto& outputAttribute = outputNode->GetOutputAttribute(output);


        if (!AreAttributesCompatible(inputAttribute, outputAttribute))
        {
            INS_INFO("Type mismatch between attributes {0} and {1}", inputAttribute.GetName(),
                            outputAttribute.GetName());

            return;
        }


        if (IsInputAttributeConnected(inputAttribute.GetId()))
        {
            INS_INFO("This input already has a connection");

            for (auto i = 0; i < m_InputLinks.size(); i++)
            {
                if (m_InputLinks[i].SourceAttribute == inputAttribute.GetId())
                {
                    RemoveLink(i);
                    break;
                }
            }
        }

        m_InputLinks.push_back({
            .SourceNode = inputNode->GetId(),
            .SourceAttribute = input,
            .DestinationNode = outputNode->GetId(),
            .DestinationAttribute = output
        });

        inputAttribute.SetConnected(true);
        outputAttribute.SetConnected(true);

        inputNode->InputAttributeConnected(inputAttribute, outputAttribute);
        outputNode->OutputAttributeConnected(outputAttribute, inputAttribute);
    }

    void Graph::ValidateLinks()
    {
        vector<i32> linksToRemove{};

        do
        {
            auto offset = 0;

            for (auto linkIndex : linksToRemove)
            {
                RemoveLink(linkIndex - offset);
                offset += 1;
            }

            linksToRemove.clear();

            for (auto i = 0; i < m_InputLinks.size(); i++)
            {
                auto& link = m_InputLinks[i];

                auto inputNode = GetNode(link.SourceNode);
                auto outputNode = GetNode(link.DestinationNode);

                if (!inputNode || !outputNode)
                {
                    linksToRemove.push_back(i);
                    continue;
                }

                auto& inputAttribute = inputNode->GetInputAttribute(link.SourceAttribute);
                auto& outputAttribute = outputNode->GetOutputAttribute(link.DestinationAttribute);

                if (inputAttribute.IsHidden() || outputAttribute.IsHidden() || !AreAttributesCompatible(inputAttribute, outputAttribute))
                {
                    linksToRemove.push_back(i);
                }
            }
        } while (!linksToRemove.empty());
    }

    bool Graph::IsInputAttribute(const Id attribute) const
    {
        for (auto& node : m_Nodes)
        {
            for (auto& input : node->GetInputAttributes())
            {
                if (input.GetId() == attribute)
                {
                    return true;
                }
            }
        }

        return false;
    }

    Node* Graph::GetNode(const Id id) const
    {
        for (auto& node : m_Nodes)
        {
            if (node->GetId() == id)
            {
                return node;
            }
        }

        return nullptr;
    }

    Node* Graph::GetNodeContainingAttribute(const Id id) const
    {
        for (auto& node : m_Nodes)
        {
            for (auto& input : node->GetInputAttributes())
            {
                if (input.GetId() == id)
                {
                    return node;
                }
            }
            for (auto& output : node->GetOutputAttributes())
            {
                if (output.GetId() == id)
                {
                    return node;
                }
            }
        }

        return nullptr;
    }

    bool Graph::IsInputAttributeConnected(const Id inputAttributeId) const
    {
        for (auto& link : m_InputLinks)
        {
            if (link.SourceAttribute == inputAttributeId)
            {
                return true;
            }
        }
        return false;
    }

    bool Graph::IsOutputAttributeConnected(const Id outputAttributeId) const
    {
        for (auto& link : m_InputLinks)
        {
            if (link.DestinationAttribute == outputAttributeId)
            {
                return true;
            }
        }

        return false;
    }

    void Graph::RemoveLink(i32 inputLinkIndex)
    {
        auto link = m_InputLinks[inputLinkIndex];

        m_InputLinks.erase(m_InputLinks.begin() + inputLinkIndex);

        Node* inputNode = GetNode(link.SourceNode);
        Node* outputNode = GetNode(link.DestinationNode);

        Attribute& inputAttribute = inputNode->GetInputAttribute(link.SourceAttribute);
        Attribute& outputAttribute = outputNode->GetOutputAttribute(link.DestinationAttribute);

        inputAttribute.SetConnected(IsInputAttributeConnected(link.SourceAttribute));
        outputAttribute.SetConnected(IsOutputAttributeConnected(link.DestinationAttribute));

        inputNode->InputAttributeDisconnected(inputAttribute, outputAttribute);
        outputNode->OutputAttributeDisconnected(outputAttribute, inputAttribute);
    }

    json Graph::Serialize()
    {
        json graphData;

        vector<json> nodes;
        vector<json> links;

        for (auto& node : m_Nodes)
        {
            json nodeData;

            const auto position = node->GetPosition();

            nodeData["Id"] = node->GetId();
            nodeData["Type"] = node->GetNodeType();
            nodeData["Position"] = {position.x, position.y};

            vector<json> inputAttributes;
            vector<json> outputAttributes;

            for (auto& input : node->GetInputAttributes())
            {
                inputAttributes.push_back({
                    {"Id", input.GetId()},
                    {"Type", GetValueTypeName(input.GetValue().GetType())},
                    {"IsParameterized", input.IsParameterized()},
                    {"IsParameterizable", input.IsParameterizable()},
                    {"ParameterName", input.GetParameterName()},
                    {"Value", input.GetValue().Serialize()},
                    {"IsHidden", input.IsHidden()},
                    {"IsModifiable", input.IsModifiable()},
                    {"IsTypeModifiable", input.IsTypeModifiable()},
                    {"AcceptsAnyNumeric", input.AcceptsAnyNumeric()},
                    {"Pin", input.HasPin()},
                });
            }

            for (auto& output : node->GetOutputAttributes())
            {
                outputAttributes.push_back({
                    {"Id", output.GetId()},
                    {"Type", GetValueTypeName(output.GetValue().GetType())},
                    {"IsParameterized", output.IsParameterized()},
                    {"IsParameterizable", output.IsParameterizable()},
                    {"ParameterName", output.GetParameterName()},
                    {"Value", output.GetValue().Serialize()},
                    {"IsHidden", output.IsHidden()},
                    {"IsModifiable", output.IsModifiable()},
                    {"IsTypeModifiable", output.IsTypeModifiable()},
                    {"AcceptsAnyNumeric", output.AcceptsAnyNumeric()},
                    {"Pin", output.HasPin()},
                });
            }

            nodeData["InputAttributes"] = inputAttributes;
            nodeData["OutputAttributes"] = outputAttributes;

            nodes.push_back(nodeData);
        }

        for (const auto& link : m_InputLinks)
        {
            links.push_back({
                {"SourceNode", link.SourceNode},
                {"SourceAttribute", link.SourceAttribute},
                {"DestinationNode", link.DestinationNode},
                {"DestinationAttribute", link.DestinationAttribute}
            });
        }

        graphData["Nodes"] = nodes;
        graphData["Links"] = links;

        return graphData;
    }

    void Graph::Deserialize(json::const_reference value, const NodeFactory& factory)
    {
        for (auto& node : m_Nodes)
        {
            node->Dispose();

            delete node;
        }

        m_Nodes.clear();

        m_InputLinks.clear();

        Id largestId = 0;

        for (const auto& nodeData : value["Nodes"])
        {
            auto node = factory.CreateNode(*this, nodeData["Type"]);

            node->SetId(nodeData["Id"]);
            node->SetPosition({nodeData["Position"][0].get<f32>(), nodeData["Position"][1].get<f32>()});

            largestId = std::max(largestId, node->GetId());

            i32 inputAttributeIndex = 0;

            for (const auto& inputAttributeData : nodeData["InputAttributes"])
            {
                auto& inputAttribute = node->GetInputAttributeAt(inputAttributeIndex++);

                inputAttribute.SetId(inputAttributeData["Id"]);
                inputAttribute.GetValue().Deserialize(inputAttributeData["Value"]);
                inputAttribute.SetParameterized(inputAttributeData["IsParameterized"]);
                inputAttribute.SetParameterizable(inputAttributeData["IsParameterizable"]);
                inputAttribute.SetParameterName(inputAttributeData["ParameterName"]);
                inputAttribute.SetHidden(inputAttributeData["IsHidden"]);
                inputAttribute.SetModifiable(inputAttributeData["IsModifiable"]);
                inputAttribute.SetTypeModifiable(inputAttributeData["IsTypeModifiable"]);
                inputAttribute.SetAcceptsAnyNumeric(inputAttributeData["AcceptsAnyNumeric"]);
                inputAttribute.SetPin(inputAttributeData["Pin"]);

                largestId = std::max(largestId, inputAttribute.GetId());
            }

            i32 outputAttributeIndex = 0;

            for (const auto& outputAttributeData : nodeData["OutputAttributes"])
            {
                auto& outputAttribute = node->GetOutputAttributeAt(outputAttributeIndex++);

                outputAttribute.SetId(outputAttributeData["Id"]);
                outputAttribute.GetValue().Deserialize(outputAttributeData["Value"]);
                outputAttribute.SetParameterized(outputAttributeData["IsParameterized"]);
                outputAttribute.SetParameterizable(outputAttributeData["IsParameterizable"]);
                outputAttribute.SetParameterName(outputAttributeData["ParameterName"]);
                outputAttribute.SetHidden(outputAttributeData["IsHidden"]);
                outputAttribute.SetModifiable(outputAttributeData["IsModifiable"]);
                outputAttribute.SetTypeModifiable(outputAttributeData["IsTypeModifiable"]);
                outputAttribute.SetAcceptsAnyNumeric(outputAttributeData["AcceptsAnyNumeric"]);
                outputAttribute.SetPin(outputAttributeData["Pin"]);

                largestId = std::max(largestId, outputAttribute.GetId());
            }

            AddNode(node);
        }

        for (const auto& linkData : value["Links"])
        {
            m_InputLinks.push_back({
                .SourceNode = linkData["SourceNode"],
                .SourceAttribute = linkData["SourceAttribute"],
                .DestinationNode = linkData["DestinationNode"],
                .DestinationAttribute = linkData["DestinationAttribute"]
            });

            GetNode(linkData["SourceNode"])->GetInputAttribute(linkData["SourceAttribute"]).SetConnected(true);
            GetNode(linkData["DestinationNode"])->GetOutputAttribute(linkData["DestinationAttribute"]).SetConnected(true);
        }

        m_IdCounter = largestId;
    }

    Value Graph::DeserializeValue(ValueType type, json::const_reference data, json::const_reference minData,
                                  json::const_reference maxData)
    {
        switch (type)
        {
        case ValueType::Float32:
            return Value(data.get<f32>(), minData.get<f32>(), maxData.get<f32>());
        case ValueType::Float32Vector2:
            return Value(vec2(data[0].get<f32>(), data[1].get<f32>()),
                         vec2(minData[0].get<f32>(), minData[1].get<f32>()),
                         vec2(maxData[0].get<f32>(), maxData[1].get<f32>()));
        case ValueType::Float32Vector3:
            return Value(vec3(data[0].get<f32>(), data[1].get<f32>(), data[2].get<f32>()),
                         vec3(minData[0].get<f32>(), minData[1].get<f32>(), minData[2].get<f32>()),
                         vec3(maxData[0].get<f32>(), maxData[1].get<f32>(), maxData[2].get<f32>()));
        case ValueType::Float32Vector4:
            return Value(vec4(data[0].get<f32>(), data[1].get<f32>(), data[2].get<f32>(), data[3].get<f32>()),
                         vec4(minData[0].get<f32>(), minData[1].get<f32>(), minData[2].get<f32>(),
                              minData[3].get<f32>()), vec4(maxData[0].get<f32>(), maxData[1].get<f32>(),
                                                           maxData[2].get<f32>(), maxData[3].get<f32>()));
        case ValueType::Int32:
            return Value(data.get<i32>(), minData.get<i32>(), maxData.get<i32>());
        case ValueType::Int32Vector2:
            return Value(ivec2(data[0].get<i32>(), data[1].get<i32>()),
                         ivec2(minData[0].get<i32>(), minData[1].get<i32>()),
                         ivec2(maxData[0].get<i32>(), maxData[1].get<i32>()));
        case ValueType::Int32Vector3:
            return Value(ivec3(data[0].get<i32>(), data[1].get<i32>(), data[2].get<i32>()),
                         ivec3(minData[0].get<i32>(), minData[1].get<i32>(), minData[2].get<i32>()),
                         ivec3(maxData[0].get<i32>(), maxData[1].get<i32>(), maxData[2].get<i32>()));
        case ValueType::Int32Vector4:
            return Value(ivec4(data[0].get<i32>(), data[1].get<i32>(), data[2].get<i32>(), data[3].get<i32>()),
                         ivec4(minData[0].get<i32>(), minData[1].get<i32>(), minData[2].get<i32>(),
                               minData[3].get<i32>()),
                         ivec4(maxData[0].get<i32>(), maxData[1].get<i32>(), maxData[2].get<i32>(),
                               maxData[3].get<i32>()));
        case ValueType::Uint32:
            return Value(data.get<u32>(), minData.get<u32>(), maxData.get<u32>());
        case ValueType::Uint32Vector2:
            return Value(uvec2(data[0].get<u32>(), data[1].get<u32>()),
                         uvec2(minData[0].get<u32>(), minData[1].get<u32>()),
                         uvec2(maxData[0].get<u32>(), maxData[1].get<u32>()));
        case ValueType::Uint32Vector3:
            return Value(uvec3(data[0].get<u32>(), data[1].get<u32>(), data[2].get<u32>()),
                         uvec3(minData[0].get<u32>(), minData[1].get<u32>(), minData[2].get<u32>()),
                         uvec3(maxData[0].get<u32>(), maxData[1].get<u32>(), maxData[2].get<u32>()));
        case ValueType::Uint32Vector4:
            return Value(uvec4(data[0].get<u32>(), data[1].get<u32>(), data[2].get<u32>(), data[3].get<u32>()),
                         uvec4(minData[0].get<u32>(), minData[1].get<u32>(), minData[2].get<u32>(),
                               minData[3].get<u32>()),
                         uvec4(maxData[0].get<u32>(), maxData[1].get<u32>(), maxData[2].get<u32>(),
                               maxData[3].get<u32>()));
        case ValueType::Texture2D:
            AssetHandle handle = AssetHandle(data.get<string>());
            auto texture = Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(handle, {});

            return Value(new TextureRef(texture));
        }
    }

    bool Graph::AreAttributesCompatible(const Attribute& input, const Attribute& output)
    {
        if (input.AcceptsAnyNumeric() && output.GetValue().IsNumeric())
        {
            return true;
        }

        return input.GetValue().CanAccept(output.GetValue());
    }

    vector<Attribute*> Graph::GetParameterizedAttributes() const
    {
        vector<Attribute*> attributes{};

        for (auto& node : m_Nodes)
        {
            for (auto& input : node->GetInputAttributes())
            {
                if (input.IsParameterized() && !input.IsConnected())
                {
                    attributes.push_back(&input);
                }
            }
            for (auto& output : node->GetOutputAttributes())
            {
                if (output.IsParameterized())
                {
                    attributes.push_back(&output);
                }
            }
        }

        return attributes;
    }
} // Portent
