#pragma once

#include "TextureRef.h"
#include "Insight/Math.h"
#include "Insight/Assets/Material.h"

namespace Portent::NodeGraph
{
    enum class ValueType : u32
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
        Float32Vector4 = 1 << 11,
        Texture2D = 1 << 12
    };

    static inline const std::pair<ValueType, string> s_ValueTypeNames[] = {
        {ValueType::Int32, "Int32"},
        {ValueType::Uint32, "Uint32"},
        {ValueType::Float32, "Float32"},
        {ValueType::Int32Vector2, "Int32Vector2"},
        {ValueType::Uint32Vector2, "Uint32Vector2"},
        {ValueType::Float32Vector2, "Float32Vector2"},
        {ValueType::Int32Vector3, "Int32Vector3"},
        {ValueType::Uint32Vector3, "Uint32Vector3"},
        {ValueType::Float32Vector3, "Float32Vector3"},
        {ValueType::Int32Vector4, "Int32Vector4"},
        {ValueType::Uint32Vector4, "Uint32Vector4"},
        {ValueType::Float32Vector4, "Float32Vector4"},
        {ValueType::Texture2D, "Texture2D"}
    };

    const string& GetValueTypeName(ValueType valueType);
    ValueType GetValueType(const string& name);

    union ValueData
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
        TextureRef* Texture2D;
    };

    class Value
    {
    public:
        explicit Value() = default;
        Value(Value&&) noexcept = default;
        Value(const Value&) = default;

        Value& operator=(const Value& other) = default;
        void SetType(ValueType value);


        explicit Value(const ValueType& type) : m_Type(type)
        {
        };
        explicit Value(const i32 value) : m_Type(ValueType::Int32) { m_Data.Int32 = value; };

        Value(const i32 value, const i32 minValue, const i32 maxValue) : m_Type(ValueType::Int32),
                                                                         m_MinValue({.Int32 = minValue}),
                                                                         m_MaxValue({.Int32 = maxValue})
        {
            m_Data.Int32 = value;
        };
        explicit Value(const u32 value) : m_Type(ValueType::Uint32) { m_Data.Uint32 = value; };

        Value(const u32 value, const u32 minValue, const u32 maxValue) : m_Type(ValueType::Uint32),
                                                                         m_MinValue({.Uint32 = minValue}),
                                                                         m_MaxValue({.Uint32 = maxValue})
        {
            m_Data.Uint32 = value;
        };
        explicit Value(const float value) : m_Type(ValueType::Float32) { m_Data.Float32 = value; };

        Value(const float value, const float minValue, const float maxValue) : m_Type(ValueType::Float32),
                                                                               m_MinValue({.Float32 = minValue}),
                                                                               m_MaxValue({.Float32 = maxValue})
        {
            m_Data.Float32 = value;
        };
        explicit Value(const ivec2 value) : m_Type(ValueType::Int32Vector2) { m_Data.Int32Vector2 = value; };

        Value(const ivec2 value, const ivec2 minValue, const ivec2 maxValue) : m_Type(ValueType::Int32Vector2),
                                                                               m_MinValue({.Int32Vector2 = minValue}),
                                                                               m_MaxValue({.Int32Vector2 = maxValue})
        {
            m_Data.Int32Vector2 = value;
        };
        explicit Value(const uvec2 value) : m_Type(ValueType::Uint32Vector2) { m_Data.Uint32Vector2 = value; };

        Value(const uvec2 value, const uvec2 minValue, const uvec2 maxValue) : m_Type(ValueType::Uint32Vector2),
                                                                               m_MinValue({.Uint32Vector2 = minValue}),
                                                                               m_MaxValue({.Uint32Vector2 = maxValue})
        {
            m_Data.Uint32Vector2 = value;
        };
        explicit Value(const vec2 value) : m_Type(ValueType::Float32Vector2) { m_Data.Float32Vector2 = value; };

        Value(const vec2 value, const vec2 minValue, const vec2 maxValue) : m_Type(ValueType::Float32Vector2),
                                                                            m_MinValue({.Float32Vector2 = minValue}),
                                                                            m_MaxValue({.Float32Vector2 = maxValue})
        {
            m_Data.Float32Vector2 = value;
        };
        explicit Value(const ivec3 value) : m_Type(ValueType::Int32Vector3) { m_Data.Int32Vector3 = value; };

        Value(const ivec3 value, const ivec3 minValue, const ivec3 maxValue) : m_Type(ValueType::Int32Vector3),
                                                                               m_MinValue({.Int32Vector3 = minValue}),
                                                                               m_MaxValue({.Int32Vector3 = maxValue})
        {
            m_Data.Int32Vector3 = value;
        };
        explicit Value(const uvec3 value) : m_Type(ValueType::Uint32Vector3) { m_Data.Uint32Vector3 = value; };

        Value(const uvec3 value, const uvec3 minValue, const uvec3 maxValue) : m_Type(ValueType::Uint32Vector3),
                                                                               m_MinValue({.Uint32Vector3 = minValue}),
                                                                               m_MaxValue({.Uint32Vector3 = maxValue})
        {
            m_Data.Uint32Vector3 = value;
        };
        explicit Value(const vec3 value) : m_Type(ValueType::Float32Vector3) { m_Data.Float32Vector3 = value; };

        Value(const vec3 value, const vec3 minValue, const vec3 maxValue) : m_Type(ValueType::Float32Vector3),
                                                                            m_MinValue({.Float32Vector3 = minValue}),
                                                                            m_MaxValue({.Float32Vector3 = maxValue})
        {
            m_Data.Float32Vector3 = value;
        };
        explicit Value(const ivec4 value) : m_Type(ValueType::Int32Vector4) { m_Data.Int32Vector4 = value; };

        Value(const ivec4 value, const ivec4 minValue, const ivec4 maxValue) : m_Type(ValueType::Int32Vector4),
                                                                               m_MinValue({.Int32Vector4 = minValue}),
                                                                               m_MaxValue({.Int32Vector4 = maxValue})
        {
            m_Data.Int32Vector4 = value;
        };
        explicit Value(const uvec4 value) : m_Type(ValueType::Uint32Vector4) { m_Data.Uint32Vector4 = value; };

        Value(const uvec4 value, const uvec4 minValue, const uvec4 maxValue) : m_Type(ValueType::Uint32Vector4),
                                                                               m_MinValue({.Uint32Vector4 = minValue}),
                                                                               m_MaxValue({.Uint32Vector4 = maxValue})
        {
            m_Data.Uint32Vector4 = value;
        };
        explicit Value(const vec4 value) : m_Type(ValueType::Float32Vector4) { m_Data.Float32Vector4 = value; };

        Value(const vec4 value, const vec4 minValue, const vec4 maxValue) : m_Type(ValueType::Float32Vector4),
                                                                            m_MinValue({.Float32Vector4 = minValue}),
                                                                            m_MaxValue({.Float32Vector4 = maxValue})
        {
            m_Data.Float32Vector4 = value;
        };
        explicit Value(TextureRef* value) : m_Type(ValueType::Texture2D), m_IsNumeric(false) { m_Data.Texture2D = value; };

        [[nodiscard]] ValueType GetType() const { return m_Type; }
        [[nodiscard]] ValueData GetData() const { return m_Data; }

        template <typename T>
        [[nodiscard]] T& GetValue() { return *reinterpret_cast<T*>(&m_Data); }

        template <typename T>
        [[nodiscard]] T& GetMinValue() { return *reinterpret_cast<T*>(&m_MinValue); }

        template <typename T>
        [[nodiscard]] T& GetMaxValue() { return *reinterpret_cast<T*>(&m_MaxValue); }

        [[nodiscard]] string GetGlslType() const;
        [[nodiscard]] bool IsNumeric() const { return m_IsNumeric; }
        bool CanAccept(const Value& value) const;
        static string GetGlslType(ValueType type);
        [[nodiscard]] string ToGlslValue();

        [[nodiscard]] bool IsSingleValue() const;
        [[nodiscard]] u32 GetComponentCount() const;
        [[nodiscard]] ValueType GetComponentType() const;
        [[nodiscard]] json Serialize(ValueData data) const;
        [[nodiscard]] json Serialize() const;
        void Deserialize(const json& data);

        [[nodiscard]] UniformValue ToUniformValue() const;

    private:
        ValueType m_Type = ValueType::Float32;
        ValueData m_Data{.Float32 = 0.0f};
        ValueData m_MinValue{.Float32 = 0.0f};
        ValueData m_MaxValue{.Float32 = FLT_MAX};

        bool m_IsNumeric = true;
    };
}
