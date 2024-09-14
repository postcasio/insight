#include "Renderer.h"

#include <imgui_internal.h>

#include "../Widgets.h"
#include "Insight/Renderer/Command.h"
#include "Insight/Ui/FontAwesome6.h"
#include "Insight/Ui/ImNodes.h"
#include "Insight/Ui/Utils.h"
#include "Insight/Ui/Widgets.h"


namespace Portent::NodeGraph
{
    static const std::pair<ValueType, const char*> s_NumericTypes[] = {
        {ValueType::Float32, "float"},
        {ValueType::Float32Vector2, "vec2"},
        {ValueType::Float32Vector3, "vec3"},
        {ValueType::Float32Vector4, "vec4"},
        {ValueType::Int32, "int"},
        {ValueType::Int32Vector2, "ivec2"},
        {ValueType::Int32Vector3, "ivec3"},
        {ValueType::Int32Vector4, "ivec4"},
        {ValueType::Uint32, "uint"},
        {ValueType::Uint32Vector2, "uvec2"},
        {ValueType::Uint32Vector3, "uvec3"},
        {ValueType::Uint32Vector4, "uvec4"}
    };

    void Renderer::RenderNode(Node* node)
    {
        auto id = node->GetId();

        if (node->ShouldUpdatePosition())
        {
            ImNodes::SetNodeGridSpacePos(id, {node->GetPosition().x, node->GetPosition().y});

            node->ShouldUpdatePosition(false);
        }

        ImNodes::BeginNode(id);

        ImNodes::BeginNodeTitleBar();
        if (Insight::Widgets::SmallButton(ICON_FA_XMARK))
        {
            m_NodeDeleteQueue.emplace(node->GetId());
        }
        ImGui::SameLine();

        ImGui::TextUnformatted(node->GetName().c_str());
        ImNodes::EndNodeTitleBar();

        RenderNodeContents(node);

        ImNodes::EndNode();

        node->m_Position = {ImNodes::GetNodeGridSpacePos(id).x, ImNodes::GetNodeGridSpacePos(id).y};
    }

    void Renderer::RenderLinks()
    {
        const auto& links = m_Graph.GetInputLinks();
        for (i32 i = 0; i < links.size(); i++)
        {
            ImNodes::Link(i, links[i].SourceAttribute, links[i].DestinationAttribute);
        }
    }

    void Renderer::SetNodePosition(Node* node, const ImVec2& position)
    {
        node->SetPosition({position.x, position.y});
    }

    void Renderer::RenderParameter(Attribute* attribute)
    {
        auto& value = attribute->GetValue();

        switch (value.GetType())
        {
        case ValueType::Float32:
            {
                auto& f = value.GetValue<float>();

                Insight::Widgets::PropertyTreeDragFloat(attribute->GetParameterName(), &f, 0.01f,
                                                        value.GetMinValue<float>(), value.GetMaxValue<float>());

                break;
            }
        case ValueType::Float32Vector2:
            {
                auto& v = value.GetValue<vec2>();

                Insight::Widgets::PropertyTreeDragFloat2(attribute->GetParameterName(), &v.x, 0.01f,
                                                         value.GetMinValue<vec2>().x, value.GetMaxValue<vec2>().x);

                break;
            }
        case ValueType::Float32Vector3:
            {
                auto& v = value.GetValue<vec3>();

                Insight::Widgets::PropertyTreeDragFloat3(attribute->GetParameterName(), &v.x, 0.01f,
                                                         value.GetMinValue<vec3>().x, value.GetMaxValue<vec3>().x);

                break;
            }
        case ValueType::Float32Vector4:
            {
                auto& v = value.GetValue<vec4>();

                Insight::Widgets::PropertyTreeDragFloat4(attribute->GetParameterName(), &v.x, 0.01f,
                                                         value.GetMinValue<vec4>().x, value.GetMaxValue<vec4>().x);

                break;
            }
        case ValueType::Texture2D:
            {
                const auto textureRef = value.GetValue<TextureRef*>();
                const auto texture = textureRef->GetTexture();

                AssetDescriptor descriptor;

                if (Widgets::PropertyTreeAssetSelector(attribute->GetParameterName(), texture,
                                                       AssetType::Texture, textureRef->GetFileBrowser(), &descriptor))
                {
                    INS_ENGINE_INFO("Asset: {0}", descriptor.Path.c_str());
                    if (descriptor.Type == AssetType::Texture)
                    {
                        textureRef->SetTexture(Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(
                            descriptor.Id, {
                                .CacheData = false
                            }));
                    }
                }

                break;
            }
        default:
            ImGui::TextUnformatted("[edit here]");
        }
    }

    void Renderer::Render()
    {
        m_NodeDeleteQueue = {};

        ImNodes::BeginNodeEditor();

        bool separator = false;
        for (auto node : m_Graph.GetNodes())
        {
            RenderNode(node);
        }

        RenderLinks();

        ImNodes::MiniMap();
    }

    void Renderer::End()
    {
        ImNodes::EndNodeEditor();

        Id first;
        Id second;
        if (ImNodes::IsLinkCreated(&first, &second))
        {
            m_Graph.AddLink(first, second);
        }

        i32 destroyed;

        if (ImNodes::IsLinkDestroyed(&destroyed))
        {
            INS_CLIENT_INFO("Link destroyed: {0}", destroyed);
            m_Graph.RemoveLink(destroyed);
        }

        while (!m_NodeDeleteQueue.empty())
        {
            const auto id = m_NodeDeleteQueue.front();
            m_NodeDeleteQueue.pop();
            m_Graph.RemoveNode(id);
            m_Graph.ValidateLinks();
        }


        m_Graph.ValidateLinks();
    }

    void Renderer::RenderNodeAttributeValue(Value& value)
    {
        constexpr float width = 200;

        switch (value.GetType())
        {
        case ValueType::Float32:
            {
                ImGui::PushItemWidth(width);
                auto& f = value.GetValue<float>();
                ImGui::DragFloat("##float", &f, 0.01f, value.GetMinValue<float>(), value.GetMaxValue<float>());

                break;
            }
        case ValueType::Float32Vector2:
            {
                ImGui::PushItemWidth(width);
                auto& v = value.GetValue<vec2>();
                ImGui::DragFloat2("##float2", &v.x, 0.01f, value.GetMinValue<vec2>().x, value.GetMaxValue<vec2>().x);

                break;
            }
        case ValueType::Float32Vector3:
            {
                ImGui::PushItemWidth(width);
                auto& v = value.GetValue<vec3>();
                ImGui::DragFloat3("##float3", &v.x, 0.01f, value.GetMinValue<vec3>().x, value.GetMaxValue<vec3>().x);

                break;
            }
        case ValueType::Float32Vector4:
            {
                ImGui::PushItemWidth(width);
                auto& v = value.GetValue<vec4>();
                ImGui::DragFloat4("##float4", &v.x, 0.01f, value.GetMinValue<vec4>().x, value.GetMaxValue<vec4>().x);

                break;
            }
        case ValueType::Texture2D:
            {
                const auto textureRef = value.GetValue<TextureRef*>();
                const auto texture = textureRef->GetTexture();

                AssetDescriptor descriptor;

                if (Widgets::AssetSelector("Texture", texture, AssetType::Texture, {64, 64}, width, textureRef->GetFileBrowser(), &descriptor))
                {
                    INS_ENGINE_INFO("Asset: {0}", descriptor.Path.c_str());
                    if (descriptor.Type == AssetType::Texture)
                    {
                        textureRef->SetTexture(
                            Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(
                                descriptor.Id, {.CacheData = false}));
                    }
                }

                break;
            }
        default:
            ImGui::TextUnformatted("[edit here]");
        }
    }

    void Renderer::RenderNodeContents(Node* node)
    {
        ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

        for (auto& input : node->GetInputAttributes())
        {
            if (input.IsHidden())
            {
                continue;
            }

            if (input.HasPin())
            {
                ImNodes::BeginInputAttribute(input.GetId());
            }

            if (!input.IsConnected() && input.IsParameterizable())
            {
                if (Insight::Widgets::SmallToggleButton(string(ICON_FA_WRENCH) + "##" + input.GetName(),
                                                        input.IsParameterized()))
                {
                    input.SetParameterized(!input.IsParameterized());
                }

                ImGui::SameLine();
            }


            ImGui::Text(input.GetName().c_str(), false);
            ImGui::SameLine();

            if ((input.IsModifiable() || input.IsTypeModifiable()) && input.AcceptsAnyNumeric())
            {
                if (ImGui::BeginCombo("##type", input.GetValue().GetGlslType().c_str()))
                {
                    for (const auto& type : s_NumericTypes)
                    {
                        if (ImGui::Selectable(type.second))
                        {
                            input.GetValue().SetType(type.first);

                            node->InputAttributeTypeChanged(input);

                            m_Graph.ValidateLinks();
                        }
                    }

                    ImGui::EndCombo();
                }
            }
            else
            {
                ImGui::TextColored({1, 1, 1, 0.5}, "%s", input.GetValue().GetGlslType().c_str());
            }

            if (input.IsModifiable() && !input.IsConnected())
            {
                if (input.IsParameterized())
                {
                    Insight::Widgets::InputText("##parameterName", input.GetParameterName(), ImGuiTextFlags_None, 200);
                }

                RenderNodeAttributeValue(input.GetValue());
            }

            if (input.HasPin())
            {
                ImNodes::EndInputAttribute();
            }
        }

        ImNodes::PopAttributeFlag();

        for (auto& output : node->GetOutputAttributes())
        {
            if (output.IsHidden())
            {
                continue;
            }

            if (output.HasPin())
            {
                ImNodes::BeginOutputAttribute(output.GetId());
            }

            if (output.IsParameterizable())
            {
                if (Insight::Widgets::SmallToggleButton(string(ICON_FA_WRENCH) + "##" + output.GetName(),
                                                        output.IsParameterized()))
                {
                    output.SetParameterized(!output.IsParameterized());
                }

                ImGui::SameLine();
            }

            ImGui::Text("%s", output.GetName().c_str());
            ImGui::SameLine();

            if ((output.IsModifiable() || output.IsTypeModifiable()) && output.AcceptsAnyNumeric())
            {
                if (ImGui::BeginCombo("##type", output.GetValue().GetGlslType().c_str()))
                {
                    for (const auto& type : s_NumericTypes)
                    {
                        if (ImGui::Selectable(type.second))
                        {
                            output.GetValue().SetType(type.first);

                            node->OutputAttributeTypeChanged(output);

                            m_Graph.ValidateLinks();
                        }
                    }

                    ImGui::EndCombo();
                }
            }
            else
            {
                ImGui::TextColored({1, 1, 1, 0.5}, "%s", output.GetValue().GetGlslType().c_str());
            }

            if (output.IsModifiable())
            {
                RenderNodeAttributeValue(output.GetValue());
            }

            if (output.HasPin())
            {
                ImNodes::EndInputAttribute();
            }
        }
    }
}
