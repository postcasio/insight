#include "Material.h"

#include "../Renderer/Command.h"
#include "../Renderer/Context.h"
#include "../Renderer/Vertex3D.h"
#include "../Renderer/SceneRenderer.h"

namespace Insight
{
    const string& GetUniformValueTypeName(UniformValueType valueType)
    {
        for (const auto& s_UniformValueTypeName : s_UniformValueTypeNames)
        {
            if (s_UniformValueTypeName.first == valueType)
            {
                return s_UniformValueTypeName.second;
            }
        }

        return s_UniformValueTypeNames[0].second;
    }

    UniformValueType GetUniformValueType(const string& name)
    {
        for (const auto& s_UniformValueTypeName : s_UniformValueTypeNames)
        {
            if (s_UniformValueTypeName.second == name)
            {
                return s_UniformValueTypeName.first;
            }
        }

        return UniformValueType::Float32;
    }

    UniformValue::UniformValue(json::const_reference j)
    {
        m_Type = GetUniformValueType(j["Type"]);

        const auto min = j["Min"];
        const auto max = j["Max"];

        const auto value = j["Value"];

        switch (m_Type)
        {
        case UniformValueType::Float32:
            m_Data.Float32 = value.get<f32>();
            m_MinValue.Float32 = min.get<f32>();
            m_MaxValue.Float32 = max.get<f32>();
            break;
        case UniformValueType::Float32Vector2:
            m_Data.Float32Vector2 = {value[0].get<f32>(), value[1].get<f32>()};
            m_MinValue.Float32Vector2 = {min[0].get<f32>(), min[1].get<f32>()};
            m_MaxValue.Float32Vector2 = {max[0].get<f32>(), max[1].get<f32>()};
            break;
        case UniformValueType::Float32Vector3:
            m_Data.Float32Vector3 = {value[0].get<f32>(), value[1].get<f32>(), value[2].get<f32>()};
            m_MinValue.Float32Vector3 = {min[0].get<f32>(), min[1].get<f32>(), min[2].get<f32>()};
            m_MaxValue.Float32Vector3 = {max[0].get<f32>(), max[1].get<f32>(), max[2].get<f32>()};
            break;
        case UniformValueType::Float32Vector4:
            m_Data.Float32Vector4 = {value[0].get<f32>(), value[1].get<f32>(), value[2].get<f32>(), value[3].get<f32>()};
            m_MinValue.Float32Vector4 = {min[0].get<f32>(), min[1].get<f32>(), min[2].get<f32>(), min[3].get<f32>()};
            m_MaxValue.Float32Vector4 = {max[0].get<f32>(), max[1].get<f32>(), max[2].get<f32>(), max[3].get<f32>()};
            break;
        case UniformValueType::Int32:
            m_Data.Int32 = value.get<i32>();
            m_MinValue.Int32 = min.get<i32>();
            m_MaxValue.Int32 = max.get<i32>();
            break;
        case UniformValueType::Int32Vector2:
            m_Data.Int32Vector2 = {value[0].get<i32>(), value[1].get<i32>()};
            m_MinValue.Int32Vector2 = {min[0].get<i32>(), min[1].get<i32>()};
            m_MaxValue.Int32Vector2 = {max[0].get<i32>(), max[1].get<i32>()};
            break;
        case UniformValueType::Int32Vector3:
            m_Data.Int32Vector3 = {value[0].get<i32>(), value[1].get<i32>(), value[2].get<i32>()};
            m_MinValue.Int32Vector3 = {min[0].get<i32>(), min[1].get<i32>(), min[2].get<i32>()};
            m_MaxValue.Int32Vector3 = {max[0].get<i32>(), max[1].get<i32>(), max[2].get<i32>()};
            break;
        case UniformValueType::Int32Vector4:
            m_Data.Int32Vector4 = {value[0].get<i32>(), value[1].get<i32>(), value[2].get<i32>(), value[3].get<i32>()};
            m_MinValue.Int32Vector4 = {min[0].get<i32>(), min[1].get<i32>(), min[2].get<i32>(), min[3].get<i32>()};
            m_MaxValue.Int32Vector4 = {max[0].get<i32>(), max[1].get<i32>(), max[2].get<i32>(), max[3].get<i32>()};
            break;
        case UniformValueType::Uint32:
            m_Data.Uint32 = value.get<u32>();
            m_MinValue.Uint32 = min.get<u32>();
            m_MaxValue.Uint32 = max.get<u32>();
            break;
        case UniformValueType::Uint32Vector2:
            m_Data.Uint32Vector2 = {value[0].get<u32>(), value[1].get<u32>()};
            m_MinValue.Uint32Vector2 = {min[0].get<u32>(), min[1].get<u32>()};
            m_MaxValue.Uint32Vector2 = {max[0].get<u32>(), max[1].get<u32>()};
            break;
        case UniformValueType::Uint32Vector3:
            m_Data.Uint32Vector3 = {value[0].get<u32>(), value[1].get<u32>(), value[2].get<u32>()};
            m_MinValue.Uint32Vector3 = {min[0].get<u32>(), min[1].get<u32>(), min[2].get<u32>()};
            m_MaxValue.Uint32Vector3 = {max[0].get<u32>(), max[1].get<u32>(), max[2].get<u32>()};
            break;
        case UniformValueType::Uint32Vector4:
            m_Data.Uint32Vector4 = {value[0].get<u32>(), value[1].get<u32>(), value[2].get<u32>(), value[3].get<u32>()};
            m_MinValue.Uint32Vector4 = {min[0].get<u32>(), min[1].get<u32>(), min[2].get<u32>(), min[3].get<u32>()};
            m_MaxValue.Uint32Vector4 = {max[0].get<u32>(), max[1].get<u32>(), max[2].get<u32>(), max[3].get<u32>()};
            break;

        }
    }

    u32 UniformValue::GetSize() const
    {
        for (const auto& s_UniformValueTypeSize : s_UniformValueTypeSizes)
        {
            if (s_UniformValueTypeSize.first == m_Type)
            {
                return s_UniformValueTypeSize.second;
            }
        }

        return sizeof(void*);
    }

    u32 UniformValue::GetAlignment() const
    {
        for (const auto& s_UniformValueTypeAlignment : s_UniformValueTypeAlignments)
        {
            if (s_UniformValueTypeAlignment.first == m_Type)
            {
                return s_UniformValueTypeAlignment.second;
            }
        }

        return 1;
    }


    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const UniformValue& value)
    {
        const auto& data = value.GetData();

        switch (value.GetType())
        {
        case UniformValueType::Float32:
            SetUniformValue(uniform, data.Float32);
            break;
        case UniformValueType::Float32Vector2:
            SetUniformValue(uniform, data.Float32Vector2);
            break;
        case UniformValueType::Float32Vector3:
            SetUniformValue(uniform, data.Float32Vector3);
            break;
        case UniformValueType::Float32Vector4:
            SetUniformValue(uniform, data.Float32Vector4);
            break;
        case UniformValueType::Int32:
            SetUniformValue(uniform, data.Int32);
            break;
        case UniformValueType::Int32Vector2:
            SetUniformValue(uniform, data.Int32Vector2);
            break;
        case UniformValueType::Int32Vector3:
            SetUniformValue(uniform, data.Int32Vector3);
            break;
        case UniformValueType::Int32Vector4:
            SetUniformValue(uniform, data.Int32Vector4);
            break;
        case UniformValueType::Uint32:
            SetUniformValue(uniform, data.Uint32);
            break;
        case UniformValueType::Uint32Vector2:
            SetUniformValue(uniform, data.Uint32Vector2);
            break;
        case UniformValueType::Uint32Vector3:
            SetUniformValue(uniform, data.Uint32Vector3);
            break;
        case UniformValueType::Uint32Vector4:
            SetUniformValue(uniform, data.Uint32Vector4);
            break;
        }
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const f32& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(f32));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::vec2& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::vec2));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::vec3& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::vec3));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::vec4& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::vec4));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const i32& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(i32));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::ivec2& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::ivec2));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::ivec3& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::ivec3));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::ivec4& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::ivec4));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const u32& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(u32));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::uvec2& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::uvec2));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::uvec3& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::uvec3));
    }

    void Material::SetUniformValue(const MaterialUniformInfo& uniform, const glm::uvec4& value)
    {
        memcpy(m_MaterialData + uniform.Offset, &value, sizeof(glm::uvec4));
    }

    Material::Material(const MaterialInfo& info) : m_Name(info.Name), m_UniformBufferSize(info.UniformBufferSize)
    {
        m_HasUniforms = info.UniformBufferSize > 0;
        m_HasSamplers = !info.Textures.empty();
        m_HasDescriptorSet = m_HasUniforms || m_HasSamplers;

        vector<vk::DescriptorSetLayoutBinding> bindings{};

        vector<Renderer::DescriptorImageInfo> imageInfos{};

        if (m_HasUniforms)
        {
            bindings.push_back({
                // Material data
                .binding = 0,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .descriptorCount = 1,
                .stageFlags = vk::ShaderStageFlagBits::eFragment,
            });

            m_MaterialData = static_cast<u8*>(malloc(m_UniformBufferSize));
            memset(m_MaterialData, 0, m_UniformBufferSize);

            for (const auto& uniform : info.Uniforms)
            {
                m_Uniforms.push_back(uniform);
            }
        }

        u32 imageBindingOffset = m_HasUniforms ? 1 : 0;

        for (u32 i = 0; i < info.Textures.size(); i++)
        {
            bindings.push_back({
                .binding = i + imageBindingOffset,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .descriptorCount = 1,
                .stageFlags = vk::ShaderStageFlagBits::eFragment,
            });

            imageInfos.push_back({
                .ImageView = *info.Textures[i]->GetImageView(),
                .Sampler = *info.Textures[i]->GetSampler()
            });
        }

        vector<Renderer::PipelineLayoutDescriptorSetLayoutInfo> descriptorSetLayouts{
            {.Layout = Renderer::Context::Instance().GetSceneDescriptorSetLayout()}
        };

        if (m_HasDescriptorSet)
        {
            m_DescriptorSetLayout = Renderer::DescriptorSetLayout::Create({
                .Name = "Material",
                .Bindings = bindings
            });

            descriptorSetLayouts.push_back({.Layout = *m_DescriptorSetLayout});
        }

        m_PipelineLayout = Renderer::PipelineLayout::Create({
            .Name = "Material",
            .DescriptorSetLayouts = descriptorSetLayouts,
            .PushConstantRanges = {
                {
                    .Stages = vk::ShaderStageFlagBits::eVertex,
                    .Offset = 0,
                    .Size = sizeof(ModelPushConstants)
                }
            }
        });

        m_Pipeline = Renderer::DynamicGraphicsPipeline::Create({
            .Name = "SceneRenderer GBuffer",
            .ColorAttachments = {
                {
                    // Position
                    .Format = vk::Format::eR32G32B32A32Sfloat
                },
                {
                    // Normal
                    .Format = vk::Format::eR32G32B32A32Sfloat
                },
                {
                    // ORM
                    .Format = vk::Format::eR8G8B8A8Unorm
                },
                {
                    // Albedo
                    .Format = Renderer::Context::Instance().GetOutputFormat()
                },
            },
            .DepthAttachmentFormat = Renderer::Context::Instance().GetDepthFormat(),
            .CullMode = vk::CullModeFlagBits::eBack,
            .PipelineLayout = *m_PipelineLayout,
            .PolygonMode = vk::PolygonMode::eFill,
            .ShaderStages = {
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex, .Module = *Renderer::Shader::Create({
                        .Name = "Material (Vertex)",
                        .Path = "Engine/Shaders/GBuffer.vert.spv",
                    })
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment,
                    .Module = info.Shader
                },
            },
            .DepthCompareOp = vk::CompareOp::eLess,
            .DepthTestEnable = true,
            .DepthWriteEnable = true,
            .VertexBufferLayout = Renderer::Vertex3D::GetLayout()
        });

        if (m_HasDescriptorSet)
        {
            m_DescriptorSet = Renderer::DescriptorSet::Create({
                .Name = "Material",
                .Layout = *m_DescriptorSetLayout,
            });


            m_Textures = info.Textures;

            vector<Renderer::DescriptorSetWriteInfo> writes{};

            if (m_HasUniforms)
            {
                m_MaterialBuffer = Renderer::Buffer::Create({
                    .Name = "Material",
                    .Size = sizeof(MaterialUniformInfo),
                    .Usage = vk::BufferUsageFlagBits::eUniformBuffer,
                });

                for (const auto& uniform : m_Uniforms)
                {
                    auto& name = uniform.Name;

                    SetUniformValue(uniform, uniform.Value);
                }

                m_MaterialBuffer->Upload(std::span(m_MaterialData, m_UniformBufferSize));

                writes.push_back({
                    .Type = vk::DescriptorType::eUniformBuffer,
                    .Binding = 0,
                    .Data = vector<Renderer::DescriptorBufferInfo>{
                        {
                            .Buffer = *m_MaterialBuffer,
                            .Offset = 0,
                            .Range = sizeof(MaterialUniformInfo)
                        }
                    }
                });
            }

            if (m_HasSamplers)
            {
                writes.push_back({
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = imageBindingOffset,
                    .Data = imageInfos
                });
            }

            m_DescriptorSet->UpdateDescriptorSet({
                .Writes = writes
            });
        }
    }

    void Material::Bind(Renderer::DescriptorSet& sceneDataDescriptorSet, const Renderer::CommandBuffer& commandBuffer,
                        const ModelPushConstants& pushConstants) const
    {

        commandBuffer.BindPipeline(*m_Pipeline);

        vector<Renderer::DescriptorSetBindSetInfo> sets{{sceneDataDescriptorSet}};

        if (m_HasDescriptorSet)
        {
            Renderer::Command::SubmitResource(m_DescriptorSet);
            sets.push_back({*m_DescriptorSet});
        }

        commandBuffer.BindDescriptorSets({
            .FirstSet = 0,
            .Sets = sets,
            .PipelineLayout = *m_PipelineLayout
        });

        commandBuffer.PushConstants({
            .Data = &pushConstants,
            .Offset = 0,
            .Size = sizeof(ModelPushConstants),
            .PipelineLayout = *m_PipelineLayout,
            .StageFlags = vk::ShaderStageFlagBits::eVertex
        });
    }
}
