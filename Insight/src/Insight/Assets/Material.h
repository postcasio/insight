#pragma once
#include "Texture.h"
#include "../Renderer/PipelineLayout.h"
#include "../Renderer/DynamicGraphicsPipeline.h"


namespace Insight
{
    enum class UniformValueType : u32
    {
        Int32 = 1,
        Uint32 = 1 << 1,
        Float32 = 1 << 2,
        Int32Vector2 = 1 << 3,
        Uint32Vector2 = 1 << 4,
        Float32Vector2 = 1 << 5,
        Int32Vector3 = 1 << 6,
        Uint32Vector3 = 1 << 7,
        Float32Vector3 = 1 << 8,
        Int32Vector4 = 1 << 9,
        Uint32Vector4 = 1 << 10,
        Float32Vector4 = 1 << 11
    };

    static inline const std::pair<UniformValueType, string> s_UniformValueTypeNames[] = {
        {UniformValueType::Int32, "Int32"},
        {UniformValueType::Uint32, "Uint32"},
        {UniformValueType::Float32, "Float32"},
        {UniformValueType::Int32Vector2, "Int32Vector2"},
        {UniformValueType::Uint32Vector2, "Uint32Vector2"},
        {UniformValueType::Float32Vector2, "Float32Vector2"},
        {UniformValueType::Int32Vector3, "Int32Vector3"},
        {UniformValueType::Uint32Vector3, "Uint32Vector3"},
        {UniformValueType::Float32Vector3, "Float32Vector3"},
        {UniformValueType::Int32Vector4, "Int32Vector4"},
        {UniformValueType::Uint32Vector4, "Uint32Vector4"},
        {UniformValueType::Float32Vector4, "Float32Vector4"}
    };

    static inline const std::pair<UniformValueType, u32> s_UniformValueTypeSizes[] = {
        {UniformValueType::Int32, sizeof(i32)},
        {UniformValueType::Uint32, sizeof(u32)},
        {UniformValueType::Float32, sizeof(float)},
        {UniformValueType::Int32Vector2, sizeof(ivec2)},
        {UniformValueType::Uint32Vector2, sizeof(uvec2)},
        {UniformValueType::Float32Vector2, sizeof(vec2)},
        {UniformValueType::Int32Vector3, sizeof(ivec3)},
        {UniformValueType::Uint32Vector3, sizeof(uvec3)},
        {UniformValueType::Float32Vector3, sizeof(vec3)},
        {UniformValueType::Int32Vector4, sizeof(ivec4)},
        {UniformValueType::Uint32Vector4, sizeof(uvec4)},
        {UniformValueType::Float32Vector4, sizeof(vec4)}
    };

    static inline const std::pair<UniformValueType, u32> s_UniformValueTypeAlignments[] = {
        {UniformValueType::Int32, 4},
        {UniformValueType::Uint32, 4},
        {UniformValueType::Float32, 4},
        {UniformValueType::Int32Vector2, 8},
        {UniformValueType::Uint32Vector2, 8},
        {UniformValueType::Float32Vector2, 8},
        {UniformValueType::Int32Vector3, 16},
        {UniformValueType::Uint32Vector3, 16},
        {UniformValueType::Float32Vector3, 16},
        {UniformValueType::Int32Vector4, 16},
        {UniformValueType::Uint32Vector4, 16},
        {UniformValueType::Float32Vector4, 16}
    };

    const string& GetUniformValueTypeName(UniformValueType valueType);
    UniformValueType GetUniformValueType(const string& name);

    union UniformValueData
    {
        i32 Int32;
        u32 Uint32;
        float Float32;
        ivec2 Int32Vector2;
        uvec2 Uint32Vector2;
        vec2 Float32Vector2;
        ivec3 Int32Vector3;
        uvec3 Uint32Vector3;
        vec3 Float32Vector3;
        ivec4 Int32Vector4;
        uvec4 Uint32Vector4;
        vec4 Float32Vector4;
    };

    class UniformValue
    {
    public:
        explicit UniformValue() = default;
        UniformValue(UniformValue&&) noexcept = default;
        UniformValue(const UniformValue&) = default;

        explicit UniformValue(json::const_reference j);

        UniformValue& operator=(const UniformValue& other) = default;
        void SetType(UniformValueType value);


        explicit UniformValue(const UniformValueType& type) : m_Type(type)
        {
        };
        explicit UniformValue(const i32 value) : m_Type(UniformValueType::Int32) { m_Data.Int32 = value; };

        UniformValue(const i32 value, const i32 minValue, const i32 maxValue) : m_Type(UniformValueType::Int32),
                                                                         m_MinValue({.Int32 = minValue}),
                                                                         m_MaxValue({.Int32 = maxValue})
        {
            m_Data.Int32 = value;
        };
        explicit UniformValue(const u32 value) : m_Type(UniformValueType::Uint32) { m_Data.Uint32 = value; };

        UniformValue(const u32 value, const u32 minValue, const u32 maxValue) : m_Type(UniformValueType::Uint32),
                                                                         m_MinValue({.Uint32 = minValue}),
                                                                         m_MaxValue({.Uint32 = maxValue})
        {
            m_Data.Uint32 = value;
        };
        explicit UniformValue(const float value) : m_Type(UniformValueType::Float32) { m_Data.Float32 = value; };

        UniformValue(const float value, const float minValue, const float maxValue) : m_Type(UniformValueType::Float32),
                                                                               m_MinValue({.Float32 = minValue}),
                                                                               m_MaxValue({.Float32 = maxValue})
        {
            m_Data.Float32 = value;
        };
        explicit UniformValue(const ivec2 value) : m_Type(UniformValueType::Int32Vector2) { m_Data.Int32Vector2 = value; };

        UniformValue(const ivec2 value, const ivec2 minValue, const ivec2 maxValue) : m_Type(UniformValueType::Int32Vector2),
                                                                               m_MinValue({.Int32Vector2 = minValue}),
                                                                               m_MaxValue({.Int32Vector2 = maxValue})
        {
            m_Data.Int32Vector2 = value;
        };
        explicit UniformValue(const uvec2 value) : m_Type(UniformValueType::Uint32Vector2) { m_Data.Uint32Vector2 = value; };

        UniformValue(const uvec2 value, const uvec2 minValue, const uvec2 maxValue) : m_Type(UniformValueType::Uint32Vector2),
                                                                               m_MinValue({.Uint32Vector2 = minValue}),
                                                                               m_MaxValue({.Uint32Vector2 = maxValue})
        {
            m_Data.Uint32Vector2 = value;
        };
        explicit UniformValue(const vec2 value) : m_Type(UniformValueType::Float32Vector2) { m_Data.Float32Vector2 = value; };

        UniformValue(const vec2 value, const vec2 minValue, const vec2 maxValue) : m_Type(UniformValueType::Float32Vector2),
                                                                            m_MinValue({.Float32Vector2 = minValue}),
                                                                            m_MaxValue({.Float32Vector2 = maxValue})
        {
            m_Data.Float32Vector2 = value;
        };
        explicit UniformValue(const ivec3 value) : m_Type(UniformValueType::Int32Vector3) { m_Data.Int32Vector3 = value; };

        UniformValue(const ivec3 value, const ivec3 minValue, const ivec3 maxValue) : m_Type(UniformValueType::Int32Vector3),
                                                                               m_MinValue({.Int32Vector3 = minValue}),
                                                                               m_MaxValue({.Int32Vector3 = maxValue})
        {
            m_Data.Int32Vector3 = value;
        };
        explicit UniformValue(const uvec3 value) : m_Type(UniformValueType::Uint32Vector3) { m_Data.Uint32Vector3 = value; };

        UniformValue(const uvec3 value, const uvec3 minValue, const uvec3 maxValue) : m_Type(UniformValueType::Uint32Vector3),
                                                                               m_MinValue({.Uint32Vector3 = minValue}),
                                                                               m_MaxValue({.Uint32Vector3 = maxValue})
        {
            m_Data.Uint32Vector3 = value;
        };
        explicit UniformValue(const vec3 value) : m_Type(UniformValueType::Float32Vector3) { m_Data.Float32Vector3 = value; };

        UniformValue(const vec3 value, const vec3 minValue, const vec3 maxValue) : m_Type(UniformValueType::Float32Vector3),
                                                                            m_MinValue({.Float32Vector3 = minValue}),
                                                                            m_MaxValue({.Float32Vector3 = maxValue})
        {
            m_Data.Float32Vector3 = value;
        };
        explicit UniformValue(const ivec4 value) : m_Type(UniformValueType::Int32Vector4) { m_Data.Int32Vector4 = value; };

        UniformValue(const ivec4 value, const ivec4 minValue, const ivec4 maxValue) : m_Type(UniformValueType::Int32Vector4),
                                                                               m_MinValue({.Int32Vector4 = minValue}),
                                                                               m_MaxValue({.Int32Vector4 = maxValue})
        {
            m_Data.Int32Vector4 = value;
        };
        explicit UniformValue(const uvec4 value) : m_Type(UniformValueType::Uint32Vector4) { m_Data.Uint32Vector4 = value; };

        UniformValue(const uvec4 value, const uvec4 minValue, const uvec4 maxValue) : m_Type(UniformValueType::Uint32Vector4),
                                                                               m_MinValue({.Uint32Vector4 = minValue}),
                                                                               m_MaxValue({.Uint32Vector4 = maxValue})
        {
            m_Data.Uint32Vector4 = value;
        };
        explicit UniformValue(const vec4 value) : m_Type(UniformValueType::Float32Vector4) { m_Data.Float32Vector4 = value; };

        UniformValue(const vec4 value, const vec4 minValue, const vec4 maxValue) : m_Type(UniformValueType::Float32Vector4),
                                                                            m_MinValue({.Float32Vector4 = minValue}),
                                                                            m_MaxValue({.Float32Vector4 = maxValue})
        {
            m_Data.Float32Vector4 = value;
        };

        [[nodiscard]] UniformValueType GetType() const { return m_Type; }
        [[nodiscard]] UniformValueData GetData() const { return m_Data; }

        template <typename T>
        [[nodiscard]] T& GetValue() { return *reinterpret_cast<T*>(&m_Data); }

        template <typename T>
        [[nodiscard]] T& GetMinValue() { return *reinterpret_cast<T*>(&m_MinValue); }

        template <typename T>
        [[nodiscard]] T& GetMaxValue() { return *reinterpret_cast<T*>(&m_MaxValue); }

        [[nodiscard]] u32 GetSize() const;
        [[nodiscard]] u32 GetAlignment() const;

    private:
        UniformValueType m_Type = UniformValueType::Float32;
        UniformValueData m_Data {.Float32 = 0.0f};
        UniformValueData m_MinValue {.Float32 = 0.0f};
        UniformValueData m_MaxValue {.Float32 = FLT_MAX};
    };

    static inline const std::pair<UniformValueType, string> s_UniformTypeNames[] = {
        {UniformValueType::Int32, "Int32"},
        {UniformValueType::Uint32, "Uint32"},
        {UniformValueType::Float32, "Float32"},
        {UniformValueType::Int32Vector2, "Int32Vector2"},
        {UniformValueType::Uint32Vector2, "Uint32Vector2"},
        {UniformValueType::Float32Vector2, "Float32Vector2"},
        {UniformValueType::Int32Vector3, "Int32Vector3"},
        {UniformValueType::Uint32Vector3, "Uint32Vector3"},
        {UniformValueType::Float32Vector3, "Float32Vector3"},
        {UniformValueType::Int32Vector4, "Int32Vector4"},
        {UniformValueType::Uint32Vector4, "Uint32Vector4"},
        {UniformValueType::Float32Vector4, "Float32Vector4"}
    };


    namespace Renderer
    {
        class SceneRenderer;
    }

    struct ModelPushConstants
    {
        alignas(16) mat4 Model;
        alignas(16) mat4 Normal;
    };

    struct MaterialUniformInfo
    {
        string Name;
        UniformValue Value;
        u32 Offset;
        u32 Size;
    };

    struct MaterialInfo
    {
        string Name;
        vector<Ref<Texture>> Textures;
        Renderer::Shader &Shader;
        u32 UniformBufferSize = 0;
        vector<MaterialUniformInfo> Uniforms;
    };

    class Material : public Asset
    {
    public:
        static Ref<Material> Create(const MaterialInfo& info)
        {
            return CreateRef<Material>(info);
        }

        void SetUniformValue(const MaterialUniformInfo& uniform, const UniformValue& value);

        void SetUniformValue(const MaterialUniformInfo& uniform, const f32& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const i32& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const u32& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const vec2& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const ivec2& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const uvec2& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const vec3& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const ivec3& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const uvec3& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const vec4& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const ivec4& value);
        void SetUniformValue(const MaterialUniformInfo& uniform, const uvec4& value);


        explicit Material(const MaterialInfo& info);
        ~Material() override {
            free(m_MaterialData);
        }

        void Bind(Renderer::DescriptorSet& sceneDataDescriptorSet, const Renderer::CommandBuffer& commandBuffer, const ModelPushConstants
                  & pushConstants) const;

        [[nodiscard]] const string& GetName() const { return m_Name; }
        [[nodiscard]] AssetType GetType() const override { return AssetType::Material; }

    private:
        string m_Name;
        bool m_HasSamplers;
        bool m_HasUniforms;
        bool m_HasDescriptorSet;

        vector<Ref<Texture>> m_Textures;

        u8* m_MaterialData;
        u32 m_UniformBufferSize;
        Ref<Renderer::Buffer> m_MaterialBuffer;

        Unique<Renderer::PipelineLayout> m_PipelineLayout;
        Unique<Renderer::DynamicGraphicsPipeline> m_Pipeline;
        Unique<Renderer::Shader> m_Shader;
        Unique<Renderer::DescriptorSetLayout> m_DescriptorSetLayout;
        Ref<Renderer::DescriptorSet> m_DescriptorSet;
        vector<MaterialUniformInfo> m_Uniforms;
    };

}
