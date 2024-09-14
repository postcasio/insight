#include "GlslGraphCompiler.h"
#include "../Nodes/MaterialNode.h"
#include <subprocess.h>

namespace Portent::Editors::MaterialEditorCompiler::GlslGraphCompiler
{
    string GlslGraphCompiler::CompileSource(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* outputNode)
    {
        INS_ENGINE_INFO("Visit node");
        VisitNode(graph, outputNode);

        string source = "#version 450\n";

        for (auto include : m_Includes)
        {
            source += fmt::format("#include \"{0}\"\n", include);
        }

        u32 firstSamplerBinding = 0;

        if (!m_Uniforms.empty())
        {
            source += "layout(std140, set = 1, binding = 0) uniform MaterialUniform {";

            for (const auto& uniform : m_Uniforms)
            {
                source += fmt::format("{0} {1};\n", NodeGraph::Value::GetGlslType(uniform.Value.GetType()), uniform.Name);
            }

            source += "} Material;\n";

            firstSamplerBinding = 1;
        }

        for (const auto& sampler : m_SamplerBindings)
        {
            source += fmt::format("layout(set = 1, binding = {0}) uniform sampler2D {1};\n", sampler.Id + firstSamplerBinding, sampler.Name);
        }

        source += "void main() {\n";

        for (const auto& code : m_Code)
        {
            source += code;
        }

        source += "}\n";

        return source;
    }

    vector<u8> GlslGraphCompiler::CompileSpirV(const string& glslSource)
    {
        INS_CLIENT_INFO(glslSource);
        const char* commandLine[] = {"/opt/homebrew/bin/glslc", "-IEngine/Shaders", "-fshader-stage=fragment", "-o", "-", "-", NULL};
        subprocess_s subprocess;
        int result = subprocess_create(commandLine, 0, &subprocess);

        if (result != 0)
        {
            throw std::runtime_error(fmt::format("SPIRV compilation failed: Error {0}", "Can't create process"));
        }

        FILE* pipe = subprocess_stdin(&subprocess);
        fputs(glslSource.c_str(), pipe);

        fclose(pipe);

        FILE* stdoutPipe = subprocess_stdout(&subprocess);
        vector<u8> stdoutData{};

        while (!feof(stdoutPipe))
        {
            u8 buffer[128] = {};
            const auto bytes = fread(buffer, 1, sizeof(buffer), stdoutPipe);

            stdoutData.insert(stdoutData.end(), buffer, buffer + bytes);
        }

        fclose(stdoutPipe);

        FILE* stderrPipe = subprocess_stderr(&subprocess);
        vector<i8> stderrData{};

        while (!feof(stderrPipe))
        {
            i8 buffer[128] = {};
            const auto bytes = fread(buffer, 1, sizeof(buffer), stderrPipe);

            stderrData.insert(stderrData.end(), buffer, buffer + bytes);
        }

        fclose(stderrPipe);

        int process_return;
        result = subprocess_join(&subprocess, &process_return);

        if (stdoutData.empty())
        {
            throw std::runtime_error(fmt::format("SPIRV compilation failed: Error {0}\n{1}", process_return, reinterpret_cast<char*>(stderrData.data())));
        }

        return stdoutData;
    }

    bool GlslGraphCompiler::Compile(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* outputNode)
    {
        string glslSource = CompileSource(graph, outputNode);

        m_ShaderBinary = CompileSpirV(glslSource);

        return true;
    }

    i32 GlslGraphCompiler::CreateVariable(const string& name, NodeGraph::ValueType type)
    {
        const i32 id = m_Variables.size();
        string uniqueName = fmt::format("_v{0}_{1}", id, name);
        std::ranges::replace(uniqueName, ' ', '_');
        INS_CLIENT_INFO("Create variable {0}: {1}", id, uniqueName);

        m_Variables.push_back({id, uniqueName, type});
        return id;
    }

    GlslUniformId GlslGraphCompiler::CreateUniform(const string& name, NodeGraph::Value value)
    {
        const i32 id = m_Uniforms.size();
        string uniqueName = fmt::format("_u{0}_{1}", id, name);
        std::ranges::replace(uniqueName, ' ', '_');
        INS_CLIENT_INFO("Create uniform {0}: {1}", id, uniqueName);

        m_Uniforms.push_back({id, uniqueName, value});
        return id;
    }

    GlslSamplerBindingId GlslGraphCompiler::CreateSamplerBinding(const string& name, NodeGraph::ValueType type)
    {
        const i32 id = m_SamplerBindings.size();
        string uniqueName = fmt::format("_s{0}_{1}", id, name);
        std::ranges::replace(uniqueName, ' ', '_');
        INS_CLIENT_INFO("Create sampler binding {0}: {1}", id, uniqueName);

        m_SamplerBindings.push_back({id, uniqueName, type});
        return id;
    }

    void GlslGraphCompiler::VisitNode(NodeGraph::Graph& graph, MaterialEditorNodes::MaterialNode* node)
    {
        if (std::ranges::find(m_VisitedNodes, node) != m_VisitedNodes.end())
        {
            return;
        }

        m_VisitedNodes.push_back(node);

        for (auto& output : node->GetOutputAttributes())
        {
            if (output.IsParameterized() || output.GetValue().GetType() == NodeGraph::ValueType::Texture2D)
            {
                if (output.GetValue().GetType() == NodeGraph::ValueType::Texture2D)
                {
                    GlslSamplerBindingId sampler = CreateSamplerBinding(node->GetName() + "_" + output.GetName(), output.GetValue().GetType());

                    m_AttributeSamplerBindings[output.GetId()] = sampler;

                    m_TextureBindings.push_back({static_cast<u32>(sampler), output.GetValue().GetValue<NodeGraph::TextureRef*>()->GetTexture()});
                }
                else
                {
                    GlslUniformId uniform = CreateUniform(node->GetName() + "_" + output.GetName(), output.GetValue());
                    m_AttributeUniforms[output.GetId()] = uniform;
                }
            }
            else
            {
                const auto variable = CreateVariable(node->GetName() + "_" + output.GetName(), output.GetValue().GetType());

                m_AttributeVariables[output.GetId()] = variable;
            }
        }

        for (auto& input : node->GetInputAttributes())
        {
            if (input.IsConnected())
            {
                auto links = graph.GetInputLinks(input.GetId());

                for (const auto link : links)
                {
                    auto connectedNode = graph.GetNode<MaterialEditorNodes::MaterialNode>(link.DestinationNode);

                    VisitNode(graph, connectedNode);
                }
            }
            else if (input.IsParameterized() || input.GetValue().GetType() == NodeGraph::ValueType::Texture2D)
            {
                if (input.GetValue().GetType() == NodeGraph::ValueType::Texture2D)
                {
                    GlslSamplerBindingId sampler = CreateSamplerBinding(node->GetName() + "_" + input.GetName(), input.GetValue().GetType());

                    m_AttributeSamplerBindings[input.GetId()] = sampler;

                    m_TextureBindings.push_back({static_cast<u32>(sampler), input.GetValue().GetValue<NodeGraph::TextureRef*>()->GetTexture()});
                }
                else
                {
                    GlslUniformId uniform = CreateUniform(node->GetName() + "_" + input.GetName(), input.GetValue());
                    m_AttributeUniforms[input.GetId()] = uniform;
                }
            }
            else
            {
                const auto variable = CreateVariable(node->GetName() + "_" + input.GetName(), input.GetValue().GetType());

                m_AttributeVariables[input.GetId()] = variable;
            }
        }

        for (const auto& includes : node->GetIncludes())
        {
            if (std::ranges::find(m_Includes, includes) == m_Includes.end())
            {
                m_Includes.push_back(includes);
            }
        }

        m_Code.push_back(node->GetCode(*this, graph));
    }

    string GlslGraphCompiler::GetInputGlslValue(NodeGraph::Graph& graph, NodeGraph::Attribute& attribute)
    {
        if (attribute.IsParameterized() && !attribute.IsConnected())
        {
            if (attribute.GetValue().GetType() == NodeGraph::ValueType::Texture2D)
            {
                return GetAttributeSamplerBinding(attribute.GetId())->Name;
            }
            return fmt::format("Material.{0}", GetAttributeUniform(attribute.GetId())->Name);
        }

        NodeGraph::Link link{};

        return graph.TryGetInputLink(attribute.GetId(), link)
                   ? (
                        attribute.GetValue().GetType() == NodeGraph::ValueType::Texture2D
                        ? GetAttributeSamplerBinding(link.DestinationAttribute)->Name
                   : GetAttributeVariable(link.DestinationAttribute)->Name
                       )
                   : (
                       attribute.GetValue().GetType() == NodeGraph::ValueType::Texture2D
                           ? GetAttributeSamplerBinding(attribute.GetId())->Name
                           : attribute.GetValue().ToGlslValue()
                   );
    }

    string GlslGraphCompiler::GetOutputGlslValue(NodeGraph::Graph& graph, NodeGraph::Attribute& attribute,
                                                 string defaultValue)
    {
        if (attribute.IsParameterized())
        {
            return GetAttributeUniform(attribute.GetId())->Name;
        }

        if (attribute.IsModifiable())
        {
            return attribute.GetValue().ToGlslValue();
        }

        return defaultValue;
    }
} // GlslGraphCompiler
