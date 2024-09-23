#include "MaterialEditor.h"

#include <imgui_internal.h>
#include <imnodes_internal.h>

#include "Insight/Ui/ImNodes.h"

#include "MaterialEditorProperties.h"
#include "MaterialEditorPreview.h"

#include "Nodes/SurfaceOutputNode.h"
#include "Nodes/TexCoordsNode.h"
#include "Nodes/Texture2DSampleNode.h"
#include "Nodes/Texture2DNode.h"

#include "Compiler/GlslGraphCompiler.h"
#include "Nodes/AddNode.h"
#include "Nodes/ComponentsToVectorNode.h"
#include "Nodes/DotProductNode.h"
#include "Nodes/ModuloNode.h"
#include "Nodes/MultiplyNode.h"
#include "Nodes/SinNode.h"
#include "Nodes/SquareRootNode.h"
#include "Nodes/SubtractNode.h"
#include "Nodes/TimeNode.h"
#include "Nodes/UnpackNormalNode.h"
#include "Nodes/VectorToComponentsNode.h"

#include <Insight/Base64.h>

#include "Nodes/ParallaxOcclusionMappingNode.h"

namespace Portent::Editors
{
    ImGuiID MaterialEditor::SetupDockSpace(ImGuiID dockSpaceId)
    {
        ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, 0.75f, &m_LeftId, &m_RightTopId);

        ImGui::DockBuilderDockWindow(m_Title.c_str(), m_LeftId);

        ImGui::DockBuilderSplitNode(m_RightTopId, ImGuiDir_Down, 0.75f, &m_RightBottomId, &m_RightTopId);

        ImGui::DockBuilderDockWindow(m_Preview->GetTitle().c_str(), m_RightTopId);

        ImGui::DockBuilderDockWindow(m_Properties->GetTitle().c_str(), m_RightBottomId);

        return m_LeftId;
    }

    void MaterialEditor::OnAdded()
    {
        if (m_AssetDescriptor.Id == AssetHandle::Null)
        {
            AddNodeAt(m_OutputNode = new MaterialEditorNodes::SurfaceOutputNode(m_Graph), {400, 100});
        }

        Compile();

        AddPanel(m_Properties = new MaterialEditorProperties(*this));
        AddPanel(m_Preview = new MaterialEditorPreview(*this));
    }

    void MaterialEditor::LoadAsset(AssetHandle id)
    {
        auto& library = Editor::Instance().GetProject().GetLibrary();

        m_AssetDescriptor = library.GetAssetDescriptor(id);

        auto materialJson = Fs::ReadJsonFile(library.GetDirectory() / m_AssetDescriptor.Path);

        m_Graph.Deserialize(materialJson["Graph"], m_NodeFactory);

        for (const auto& node : m_Graph.GetNodes())
        {
            auto* materialNode = dynamic_cast<MaterialEditorNodes::MaterialNode*>(node);

            if (materialNode->IsOutputNode())
            {
                m_OutputNode = materialNode;
                break;
            }
        }

        m_Title = m_AssetDescriptor.Path.filename().string() + "##Material Editor " + id.ToString();
    }

    void MaterialEditor::SaveAsset(AssetDescriptor& descriptor)
    {
        const auto& library = Editor::Instance().GetProject().GetLibrary();

        json materialData;

        const auto& shaderBinary = GetShaderBinary();

        materialData["Id"] = descriptor.Id.ToString();
        materialData["Graph"] = GetGraph().Serialize();
        materialData["Shader"] = Base64::to_base64(string_view(reinterpret_cast<const char*>(shaderBinary.data()), shaderBinary.size()));

        const auto& textureBindings = GetTextureBindings();

        vector<json> textures;
        textures.resize(textureBindings.size());

        for (const auto& texture : textureBindings)
        {
            if (texture.Texture)
            {
                textures[texture.Index] = texture.Texture->GetHandle().ToString();
            }
            else
            {
                textures[texture.Index] = {};
            }
        }

        materialData["Textures"] = textures;

        materialData["Uniforms"] = json::array();

        u32 offset = 0;

        for (const auto& uniform : m_Uniforms)
        {
            const auto uniformValue = uniform.Value.ToUniformValue();

            auto size = uniformValue.GetSize();
            auto alignment = uniformValue.GetAlignment();

            if (offset % alignment != 0)
            {
                offset += alignment - (offset % alignment);
            }

            materialData["Uniforms"].push_back({
                {"Name", uniform.Name},
                {"Value", uniform.Value.Serialize()},
                {"Size", size},
                {"Offset", offset}
            });

            offset += size;
        }

        materialData["UniformBufferSize"] = offset;

        Fs::WriteJsonFile(library.GetDirectory() / descriptor.Path, materialData);

        INS_INFO("Wrote material data to {0}", descriptor.Path.c_str());
    }

    void MaterialEditor::OnRenderContents()
    {
        m_Renderer.Render();

        m_Renderer.End();

        RenderContextMenu();
    }

    void MaterialEditor::OnRenderMainMenu()
    {
        const bool hasMaterial = HasMaterial();
        auto& library = Editor::Instance().GetProject().GetLibrary();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "Ctrl+S", nullptr,
                                    m_AssetDescriptor.Id != AssetHandle::Null))
                {
                    SaveAsset(m_AssetDescriptor);
                }

                if (ImGui::MenuItem("Save As...", nullptr, nullptr, true))
                {
                    string newPath;

                    if (Editor::SaveFileDialog(newPath, Editor::Instance().GetProject().GetLibrary().GetDirectory(),
                                               {{"Material", "InsightMaterial"}}))
                    {
                        AssetDescriptor descriptor;

                        if (library.TryGetAssetDescriptor(newPath, &descriptor))
                        {
                            library.RemoveAsset(descriptor.Id);
                        }

                        descriptor = {
                            .Path = path(newPath).lexically_proximate(Editor::Instance().GetProject().GetLibrary().GetDirectory()),
                            .Type = AssetType::Material,
                            .Id = AssetHandle::CreateNew()
                        };

                        SaveAsset(descriptor);
                        library.AddAsset(descriptor);
                    }
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::BeginMenu("Environment"))
                {
                    const auto& environments = Editor::Instance().GetResources().GetHDRITextures();

                    for (const auto& environment : environments)
                    {
                        if (ImGui::MenuItem(environment->GetName().c_str()))
                        {
                            m_Preview->SetEnvironmentCubemap(environment);
                        }
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void MaterialEditor::AddNodeAt(NodeGraph::Node* node, const ImVec2& position)
    {
        m_Graph.AddNode(node);

        node->SetPosition({position.x, position.y});
    }

    void MaterialEditor::RenderContextMenu()
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            INS_INFO("Right mouse clicked");
            const auto mousePos = ImGui::GetMousePos();
            const auto windowPos = ImGui::GetWindowPos();
            const auto panning = ImNodes::GetCurrentContext()->EditorCtx->Panning;

            m_lastRightClickPosition = {mousePos.x - windowPos.x - panning.x, mousePos.y - windowPos.y - panning.y};

            ImGui::OpenPopup("context");
        }

        if (ImGui::BeginPopup("context"))
        {
            if (ImGui::BeginMenu("Add Node"))
            {
                if (ImGui::BeginMenu("Textures"))
                {
                    if (ImGui::MenuItem("Texture2D"))
                    {
                        AddNodeAt(new MaterialEditorNodes::Texture2DNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Texture2D Sampler"))
                    {
                        AddNodeAt(new MaterialEditorNodes::Texture2DSampleNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Texture Coords"))
                    {
                        AddNodeAt(new MaterialEditorNodes::TexCoordsNode(m_Graph), m_lastRightClickPosition);
                    }

                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Utility"))
                {
                    if (ImGui::MenuItem("Time"))
                    {
                        AddNodeAt(new MaterialEditorNodes::TimeNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Vector to Components"))
                    {
                        AddNodeAt(new MaterialEditorNodes::VectorToComponentsNode(m_Graph),
                                  m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Components to Vector"))
                    {
                        AddNodeAt(new MaterialEditorNodes::ComponentsToVectorNode(m_Graph),
                                  m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Parallax Occlusion Mapping"))
                    {
                        AddNodeAt(new MaterialEditorNodes::ParallaxOcclusionMappingNode(m_Graph),
                                  m_lastRightClickPosition);
                    }

                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Math"))
                {
                    if (ImGui::MenuItem("Add"))
                    {
                        AddNodeAt(new MaterialEditorNodes::AddNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Subtract"))
                    {
                        AddNodeAt(new MaterialEditorNodes::SubtractNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Multiply"))
                    {
                        AddNodeAt(new MaterialEditorNodes::MultiplyNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Modulo"))
                    {
                        AddNodeAt(new MaterialEditorNodes::ModuloNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Sin"))
                    {
                        AddNodeAt(new MaterialEditorNodes::SinNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Square Root"))
                    {
                        AddNodeAt(new MaterialEditorNodes::SquareRootNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Unpack Normal"))
                    {
                        AddNodeAt(new MaterialEditorNodes::UnpackNormalNode(m_Graph), m_lastRightClickPosition);
                    }

                    if (ImGui::MenuItem("Dot Product"))
                    {
                        AddNodeAt(new MaterialEditorNodes::DotProductNode(m_Graph), m_lastRightClickPosition);
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }


            ImGui::EndPopup();
        }
    }

    void MaterialEditor::Compile()
    {
        using Compiler = MaterialEditorCompiler::GlslGraphCompiler::GlslGraphCompiler;
        Compiler compiler;

        try
        {
            if (compiler.Compile(m_Graph, m_OutputNode))
            {
                INS_INFO("GLSL compilation successful: {0} bytes", compiler.GetShaderBinary().size());
                m_ShaderBinary = compiler.GetShaderBinary();
                m_TextureBindings = compiler.GetTextureBindings();
                m_Uniforms = compiler.GetUniforms();

                m_Shader = Renderer::Shader::Create({
                    .Name = "Preview Shader",
                    .Binary = m_ShaderBinary,
                    .EntryPoint = "main"
                });

                UpdateMaterial();
            }
        }
        catch (const std::exception& e)
        {
            INS_ERROR("GLSL compilation failed: {0}", e.what());
        }
    }

    void MaterialEditor::UpdateMaterial()
    {
        vector<Ref<Texture>> textures;

        textures.resize(m_TextureBindings.size());

        for (const auto& binding : m_TextureBindings)
        {
            if (binding.Texture == nullptr)
            {
                INS_WARN("Not all textures are bound");
                return;
            }

            textures[binding.Index] = binding.Texture;
        }

        vector<MaterialUniformInfo> uniforms;

        u32 offset = 0;

        for (const auto& uniform : m_Uniforms)
        {
            const auto uniformValue = uniform.Value.ToUniformValue();

            auto size = uniformValue.GetSize();
            auto alignment = uniformValue.GetAlignment();

            if (offset % alignment != 0)
            {
                offset += alignment - (offset % alignment);
            }

            uniforms.push_back({
                .Name = uniform.Name,
                .Value = uniformValue,
                .Offset = offset,
                .Size = size
            });

            offset += size;
        }

        m_UniformBufferSize = offset;

        const MaterialInfo info = {
            .Name = "Preview Material",
            .Textures = textures,
            .Shader = *m_Shader,
            .UniformBufferSize = m_UniformBufferSize,
            .Uniforms = uniforms
        };

        m_Material = Material::Create(info);
    }

    void MaterialEditor::UpdateModel()
    {
        m_Preview->UpdateModel();
    }
}
