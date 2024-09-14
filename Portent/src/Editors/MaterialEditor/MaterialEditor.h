#pragma once

#include "../EditorPanel.h"
#include "../../NodeGraph/Graph.h"
#include "../../NodeGraph/Renderer.h"
#include "Compiler/GlslGraphCompiler.h"
#include "Insight/Assets/Material.h"
#include "Nodes/MaterialNode.h"
#include "Nodes/MaterialNodeFactory.h"

namespace Portent::Editors
{
    class MaterialEditorPreview;
    class MaterialEditorProperties;

    class MaterialEditor final : public EditorPanel
    {
    public:
        MaterialEditor() : EditorPanel("Material Editor"), m_Renderer(m_Graph)
        {
            m_CreateDockSpaceWindow = true;
            m_DockSpaceClass = "MaterialEditor" + Uuid::CreateNew().ToString();
        };

        ~MaterialEditor() override = default;

        ImGuiID SetupDockSpace(ImGuiID dockSpaceId) override;

        void OnAdded() override;

        void LoadAsset(AssetHandle id) override;
        void SaveAsset(AssetDescriptor& descriptor) override;

        void OnRenderContents() override;
        void OnRenderMainMenu() override;
        void AddNodeAt(NodeGraph::Node* node, const ImVec2& position);
        void RenderContextMenu();

        void Compile();
        void UpdateMaterial();

        [[nodiscard]] NodeGraph::Graph& GetGraph() { return m_Graph; }
        [[nodiscard]] NodeGraph::Renderer& GetRenderer() { return m_Renderer; }

        [[nodiscard]] bool HasMaterial() const { return m_Material != nullptr; }
        [[nodiscard]] const vector<u8>& GetShaderBinary() const { return m_ShaderBinary; }
        [[nodiscard]] Ref<Material> GetMaterial() { return m_Material; }

        void UpdateModel();

        [[nodiscard]] bool HasAsset() const override { return m_Material != nullptr; }
        [[nodiscard]] Ref<Asset> GetAsset() const override { return m_Material; };
        [[nodiscard]] AssetHandle GetAssetHandle() const override { return m_Material->GetHandle(); };

        [[nodiscard]] const vector<MaterialEditorCompiler::GlslGraphCompiler::TextureBinding>& GetTextureBindings() const { return m_TextureBindings; }
        [[nodiscard]] const vector<MaterialEditorCompiler::GlslGraphCompiler::GlslUniform>& GetUniforms() const { return m_Uniforms; }

    private:
        ImGuiID m_LeftId{};
        ImGuiID m_RightTopId{};
        ImGuiID m_RightBottomId{};

        MaterialEditorProperties *m_Properties = nullptr;
        MaterialEditorPreview *m_Preview = nullptr;
        MaterialEditorNodes::MaterialNode *m_OutputNode = nullptr;

        Unique<Renderer::Shader> m_Shader;

        NodeGraph::Graph m_Graph;
        NodeGraph::Renderer m_Renderer;
        Ref<Material> m_Material;
        u32 m_UniformBufferSize = 0;
        vector<MaterialEditorCompiler::GlslGraphCompiler::TextureBinding> m_TextureBindings{};
        vector<MaterialEditorCompiler::GlslGraphCompiler::GlslUniform> m_Uniforms{};

        ImVec2 m_lastRightClickPosition {0, 0};
        AssetDescriptor m_AssetDescriptor;
        MaterialEditorNodes::MaterialNodeFactory m_NodeFactory;
        vector<u8> m_ShaderBinary;
    };
}
