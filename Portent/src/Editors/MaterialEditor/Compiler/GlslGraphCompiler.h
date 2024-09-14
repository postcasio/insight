#pragma once
#include "../../../NodeGraph/Graph.h"

namespace Portent::Editors::MaterialEditorNodes
{
    class MaterialNode;
}

namespace Portent::Editors::MaterialEditorCompiler::GlslGraphCompiler
{
    using GlslSamplerBindingId = i32;
    using GlslVariableId = i32;
    using GlslUniformId = i32;

    struct GlslSamplerBinding
    {
        GlslSamplerBindingId Id;
        string Name;
        NodeGraph::ValueType Type;
    };

    struct GlslVariable
    {
        GlslVariableId Id;
        string Name;
        NodeGraph::ValueType Type;
    };

    struct GlslUniform
    {
        GlslUniformId Id;
        string Name;
        NodeGraph::Value Value;
    };

    struct TextureBinding
    {
        u32 Index;
        Ref<Texture> Texture;
    };

    class GlslGraphCompiler
    {
    public:
        GlslGraphCompiler() = default;
        ~GlslGraphCompiler() = default;

        string CompileSource(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* outputNode);
        vector<u8> CompileSpirV(const string& glslSource);
        bool Compile(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* outputNode);

        GlslVariableId CreateVariable(const string& name, NodeGraph::ValueType type);
        GlslUniformId CreateUniform(const string& name, NodeGraph::Value value);
        GlslSamplerBindingId CreateSamplerBinding(const string& name, NodeGraph::ValueType type);

        void VisitNode(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* node);

        GlslVariable* GetAttributeVariable(const NodeGraph::Id id)
        {
            return &m_Variables[m_AttributeVariables[id]];
        }

        GlslUniform* GetAttributeUniform(const NodeGraph::Id id)
        {
            return &m_Uniforms[m_AttributeUniforms[id]];
        }

        GlslSamplerBinding* GetAttributeSamplerBinding(const NodeGraph::Id id)
        {
            return &m_SamplerBindings[m_AttributeSamplerBindings[id]];
        }

        [[nodiscard]] vector<TextureBinding>& GetTextureBindings() { return m_TextureBindings; }
        [[nodiscard]] vector<GlslUniform> &GetUniforms() { return m_Uniforms; }

        [[nodiscard]] string GetInputGlslValue(NodeGraph::Graph& graph, NodeGraph::Attribute& attribute);
        [[nodiscard]] string GetOutputGlslValue(NodeGraph::Graph& graph, NodeGraph::Attribute& attribute, string defaultValue);

        [[nodiscard]] vector<u8> &GetShaderBinary() { return m_ShaderBinary; }

    private:
        vector<GlslVariable> m_Variables{};
        vector<GlslUniform> m_Uniforms{};
        vector<GlslSamplerBinding> m_SamplerBindings;
        vector<NodeGraph::Node*> m_VisitedNodes{};
        unordered_map<NodeGraph::Id, GlslVariableId> m_AttributeVariables{};
        unordered_map<NodeGraph::Id, GlslUniformId> m_AttributeUniforms{};
        unordered_map<NodeGraph::Id, GlslSamplerBindingId> m_AttributeSamplerBindings{};

        vector<string> m_Includes{};
        vector<string> m_Code{};
        vector<u8> m_ShaderBinary{};
        vector<TextureBinding> m_TextureBindings{};
    };
} // GlslGraphCompiler
