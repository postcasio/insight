#include "Value.h"

namespace Portent::NodeGraph
{
    static const map<ValueType, vector<ValueType>> acceptTable {
        {ValueType::Float32, {ValueType::Float32}},
        {ValueType::Float32Vector2, {ValueType::Float32Vector2, ValueType::Float32}},
        {ValueType::Float32Vector3, {ValueType::Float32Vector3, ValueType::Float32}},
        {ValueType::Float32Vector4, {ValueType::Float32Vector4, ValueType::Float32}},
        {ValueType::Int32, {ValueType::Int32}},
        {ValueType::Int32Vector2, {ValueType::Int32Vector2, ValueType::Int32}},
        {ValueType::Int32Vector3, {ValueType::Int32Vector3, ValueType::Int32}},
        {ValueType::Int32Vector4, {ValueType::Int32Vector4, ValueType::Int32}},
        {ValueType::Uint32, {ValueType::Uint32}},
        {ValueType::Uint32Vector2, {ValueType::Uint32Vector2, ValueType::Uint32}},
        {ValueType::Uint32Vector3, {ValueType::Uint32Vector3, ValueType::Uint32}},
        {ValueType::Uint32Vector4, {ValueType::Uint32Vector4, ValueType::Uint32}},
        {ValueType::Texture2D, {ValueType::Texture2D}}
    };

    const string& GetValueTypeName(ValueType valueType)
    {
        for (const auto& s_ValueTypeName : s_ValueTypeNames)
        {
            if (s_ValueTypeName.first == valueType)
            {
                return s_ValueTypeName.second;
            }
        }

        return s_ValueTypeNames[0].second;
    }

    ValueType GetValueType(const string& name)
    {
        for (const auto& s_ValueTypeName : s_ValueTypeNames)
        {
            if (s_ValueTypeName.second == name)
            {
                return s_ValueTypeName.first;
            }
        }

        return ValueType::Float32;
    }

    void Value::SetType(ValueType value)
    {
        m_Type = value;
        switch (m_Type)
        {
        case ValueType::Float32:
            m_Data.Float32 = 0.0f;
            m_MinValue.Float32 = -FLT_MAX;
            m_MaxValue.Float32 = FLT_MAX;
            m_IsNumeric = true;
            break;
        case ValueType::Float32Vector2:
            m_Data.Float32Vector2 = {0.0f, 0.0f};
            m_MinValue.Float32Vector2 = {-FLT_MAX, -FLT_MAX};
            m_MaxValue.Float32Vector2 = {FLT_MAX, FLT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Float32Vector3:
            m_Data.Float32Vector3 = {0.0f, 0.0f, 0.0f};
            m_MinValue.Float32Vector3 = {-FLT_MAX, -FLT_MAX, -FLT_MAX};
            m_MaxValue.Float32Vector3 = {FLT_MAX, FLT_MAX, FLT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Float32Vector4:
            m_Data.Float32Vector4 = {0.0f, 0.0f, 0.0f, 0.0f};
            m_MinValue.Float32Vector4 = {-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX};
            m_MaxValue.Float32Vector4 = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Int32:
            m_Data.Int32 = 0;
            m_MinValue.Int32 = INT_MIN;
            m_MaxValue.Int32 = INT_MAX;
            m_IsNumeric = true;
            break;
        case ValueType::Int32Vector2:
            m_Data.Int32Vector2 = {0, 0};
            m_MinValue.Int32Vector2 = {INT_MIN, INT_MIN};
            m_MaxValue.Int32Vector2 = {INT_MAX, INT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Int32Vector3:
            m_Data.Int32Vector3 = {0, 0, 0};
            m_MinValue.Int32Vector3 = {INT_MIN, INT_MIN, INT_MIN};
            m_MaxValue.Int32Vector3 = {INT_MAX, INT_MAX, INT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Int32Vector4:
            m_Data.Int32Vector4 = {0, 0, 0, 0};
            m_MinValue.Int32Vector4 = {INT_MIN, INT_MIN, INT_MIN, INT_MIN};
            m_MaxValue.Int32Vector4 = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Uint32:
            m_Data.Uint32 = 0;
            m_MinValue.Uint32 = 0;
            m_MaxValue.Uint32 = UINT_MAX;
            m_IsNumeric = true;
            break;
        case ValueType::Uint32Vector2:
            m_Data.Uint32Vector2 = {0, 0};
            m_MinValue.Uint32Vector2 = {0, 0};
            m_MaxValue.Uint32Vector2 = {UINT_MAX, UINT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Uint32Vector3:
            m_Data.Uint32Vector3 = {0, 0, 0};
            m_MinValue.Uint32Vector3 = {0, 0, 0};
            m_MaxValue.Uint32Vector3 = {UINT_MAX, UINT_MAX, UINT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Uint32Vector4:
            m_Data.Uint32Vector4 = {0, 0, 0, 0};
            m_MinValue.Uint32Vector4 = {0, 0, 0, 0};
            m_MaxValue.Uint32Vector4 = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};
            m_IsNumeric = true;
            break;
        case ValueType::Texture2D:
            m_Data.Texture2D = nullptr;
            m_IsNumeric = false;
            break;
        }
    }

    bool Value::IsSingleValue() const
    {
        return m_Type == ValueType::Float32 || m_Type == ValueType::Int32 || m_Type == ValueType::Uint32;
    }

    u32 Value::GetComponentCount() const
    {
        switch (m_Type)
        {
        case ValueType::Float32:
        case ValueType::Int32:
        case ValueType::Uint32:
            return 1;
        case ValueType::Float32Vector2:
        case ValueType::Int32Vector2:
        case ValueType::Uint32Vector2:
            return 2;
        case ValueType::Float32Vector3:
        case ValueType::Int32Vector3:
        case ValueType::Uint32Vector3:
            return 3;
        case ValueType::Float32Vector4:
        case ValueType::Int32Vector4:
        case ValueType::Uint32Vector4:
            return 4;
        case ValueType::Texture2D:
            return 1;
        }

        return 0;
    }

    ValueType Value::GetComponentType() const
    {
        switch (m_Type)
        {
        case ValueType::Float32:
        case ValueType::Float32Vector2:
        case ValueType::Float32Vector3:
        case ValueType::Float32Vector4:
            return ValueType::Float32;
        case ValueType::Int32:
        case ValueType::Int32Vector2:
        case ValueType::Int32Vector3:
        case ValueType::Int32Vector4:
            return ValueType::Int32;
        case ValueType::Uint32:
        case ValueType::Uint32Vector2:
        case ValueType::Uint32Vector3:
        case ValueType::Uint32Vector4:
            return ValueType::Uint32;
        case ValueType::Texture2D:
            return ValueType::Texture2D;
        }
        return ValueType::Float32;
    }

    UniformValue Value::ToUniformValue() const
    {
        switch (m_Type)
        {
        case ValueType::Float32:
            return UniformValue{m_Data.Float32};
        case ValueType::Float32Vector2:
            return UniformValue{m_Data.Float32Vector2};
        case ValueType::Float32Vector3:
            return UniformValue{m_Data.Float32Vector3};
        case ValueType::Float32Vector4:
            return UniformValue{m_Data.Float32Vector4};
        case ValueType::Int32:
            return UniformValue{m_Data.Int32};
        case ValueType::Int32Vector2:
            return UniformValue{m_Data.Int32Vector2};
        case ValueType::Int32Vector3:
            return UniformValue{m_Data.Int32Vector3};
        case ValueType::Int32Vector4:
            return UniformValue{m_Data.Int32Vector4};
        case ValueType::Uint32:
            return UniformValue{m_Data.Uint32};
        case ValueType::Uint32Vector2:
            return UniformValue{m_Data.Uint32Vector2};
        case ValueType::Uint32Vector3:
            return UniformValue{m_Data.Uint32Vector3};
        case ValueType::Uint32Vector4:
            return UniformValue{m_Data.Uint32Vector4};
        case ValueType::Texture2D:
            throw std::runtime_error("Cannot convert Texture2D to UniformValue");
        }
        return UniformValue{};
    }

    void Value::Deserialize(json::const_reference data)
    {
        m_Type = GetValueType(data["Type"]);

        const auto min = data["Min"];
        const auto max = data["Max"];

        const auto value = data["Value"];

        switch (m_Type)
        {
        case ValueType::Float32:
            m_Data.Float32 = value.get<f32>();
            m_MinValue.Float32 = min.get<f32>();
            m_MaxValue.Float32 = max.get<f32>();
            break;
        case ValueType::Float32Vector2:
            m_Data.Float32Vector2 = {value[0].get<f32>(), value[1].get<f32>()};
            m_MinValue.Float32Vector2 = {min[0].get<f32>(), min[1].get<f32>()};
            m_MaxValue.Float32Vector2 = {max[0].get<f32>(), max[1].get<f32>()};
            break;
        case ValueType::Float32Vector3:
            m_Data.Float32Vector3 = {value[0].get<f32>(), value[1].get<f32>(), value[2].get<f32>()};
            m_MinValue.Float32Vector3 = {min[0].get<f32>(), min[1].get<f32>(), min[2].get<f32>()};
            m_MaxValue.Float32Vector3 = {max[0].get<f32>(), max[1].get<f32>(), max[2].get<f32>()};
            break;
        case ValueType::Float32Vector4:
            m_Data.Float32Vector4 = {value[0].get<f32>(), value[1].get<f32>(), value[2].get<f32>(), value[3].get<f32>()};
            m_MinValue.Float32Vector4 = {min[0].get<f32>(), min[1].get<f32>(), min[2].get<f32>(), min[3].get<f32>()};
            m_MaxValue.Float32Vector4 = {max[0].get<f32>(), max[1].get<f32>(), max[2].get<f32>(), max[3].get<f32>()};
            break;
        case ValueType::Int32:
            m_Data.Int32 = value.get<i32>();
            m_MinValue.Int32 = min.get<i32>();
            m_MaxValue.Int32 = max.get<i32>();
            break;
        case ValueType::Int32Vector2:
            m_Data.Int32Vector2 = {value[0].get<i32>(), value[1].get<i32>()};
            m_MinValue.Int32Vector2 = {min[0].get<i32>(), min[1].get<i32>()};
            m_MaxValue.Int32Vector2 = {max[0].get<i32>(), max[1].get<i32>()};
            break;
        case ValueType::Int32Vector3:
            m_Data.Int32Vector3 = {value[0].get<i32>(), value[1].get<i32>(), value[2].get<i32>()};
            m_MinValue.Int32Vector3 = {min[0].get<i32>(), min[1].get<i32>(), min[2].get<i32>()};
            m_MaxValue.Int32Vector3 = {max[0].get<i32>(), max[1].get<i32>(), max[2].get<i32>()};
            break;
        case ValueType::Int32Vector4:
            m_Data.Int32Vector4 = {value[0].get<i32>(), value[1].get<i32>(), value[2].get<i32>(), value[3].get<i32>()};
            m_MinValue.Int32Vector4 = {min[0].get<i32>(), min[1].get<i32>(), min[2].get<i32>(), min[3].get<i32>()};
            m_MaxValue.Int32Vector4 = {max[0].get<i32>(), max[1].get<i32>(), max[2].get<i32>(), max[3].get<i32>()};
            break;
        case ValueType::Uint32:
            m_Data.Uint32 = value.get<u32>();
            m_MinValue.Uint32 = min.get<u32>();
            m_MaxValue.Uint32 = max.get<u32>();
            break;
        case ValueType::Uint32Vector2:
            m_Data.Uint32Vector2 = {value[0].get<u32>(), value[1].get<u32>()};
            m_MinValue.Uint32Vector2 = {min[0].get<u32>(), min[1].get<u32>()};
            m_MaxValue.Uint32Vector2 = {max[0].get<u32>(), max[1].get<u32>()};
            break;
        case ValueType::Uint32Vector3:
            m_Data.Uint32Vector3 = {value[0].get<u32>(), value[1].get<u32>(), value[2].get<u32>()};
            m_MinValue.Uint32Vector3 = {min[0].get<u32>(), min[1].get<u32>(), min[2].get<u32>()};
            m_MaxValue.Uint32Vector3 = {max[0].get<u32>(), max[1].get<u32>(), max[2].get<u32>()};
            break;
        case ValueType::Uint32Vector4:
            m_Data.Uint32Vector4 = {value[0].get<u32>(), value[1].get<u32>(), value[2].get<u32>(), value[3].get<u32>()};
            m_MinValue.Uint32Vector4 = {min[0].get<u32>(), min[1].get<u32>(), min[2].get<u32>(), min[3].get<u32>()};
            m_MaxValue.Uint32Vector4 = {max[0].get<u32>(), max[1].get<u32>(), max[2].get<u32>(), max[3].get<u32>()};
            break;
        case ValueType::Texture2D:
            AssetHandle handle = AssetHandle(value.get<string>());
            auto texture = Editor::Instance().GetProject().GetLibrary().LoadAsset<Texture>(handle, {});

            if (m_Data.Texture2D)
            {
                free(m_Data.Texture2D);
            }

            m_Data.Texture2D = new TextureRef(texture);
            break;
        }
    }

    json Value::Serialize() const
    {
        if (m_Type == ValueType::Texture2D)
        {
            return {
                {"Value", Serialize(m_Data)},
                {"Min", NULL},
                {"Max", NULL},
                {"Type", GetValueTypeName(m_Type)}
            };
        }

        return {
            {"Value", Serialize(m_Data)},
            {"Min", Serialize(m_MinValue)},
            {"Max", Serialize(m_MaxValue)},
            {"Type", GetValueTypeName(m_Type)}
        };
    }

    json Value::Serialize(ValueData data) const
    {
        switch (m_Type)
        {
        case ValueType::Float32:
            return data.Float32;
        case ValueType::Float32Vector2:
            return {data.Float32Vector2.x, data.Float32Vector2.y};
        case ValueType::Float32Vector3:
            return {data.Float32Vector3.x, data.Float32Vector3.y, data.Float32Vector3.z};
        case ValueType::Float32Vector4:
            return {data.Float32Vector4.x, data.Float32Vector4.y, data.Float32Vector4.z, data.Float32Vector4.w};
        case ValueType::Int32:
            return data.Int32;
        case ValueType::Int32Vector2:
            return {data.Int32Vector2.x, data.Int32Vector2.y};
        case ValueType::Int32Vector3:
            return {data.Int32Vector3.x, data.Int32Vector3.y, data.Int32Vector3.z};
        case ValueType::Int32Vector4:
            return {data.Int32Vector4.x, data.Int32Vector4.y, data.Int32Vector4.z, data.Int32Vector4.w};
        case ValueType::Uint32:
            return data.Uint32;
        case ValueType::Uint32Vector2:
            return {data.Uint32Vector2.x, data.Uint32Vector2.y};
        case ValueType::Uint32Vector3:
            return {data.Uint32Vector3.x, data.Uint32Vector3.y, data.Uint32Vector3.z};
        case ValueType::Uint32Vector4:
            return {data.Uint32Vector4.x, data.Uint32Vector4.y, data.Uint32Vector4.z, data.Uint32Vector4.w};
        case ValueType::Texture2D:
            const auto texture = data.Texture2D->GetTexture();

            if (texture)
            {
                return texture->GetHandle().ToString();
            }

            return {};
        }

        return {};
    }

    string Value::GetGlslType() const
    {
        return GetGlslType(m_Type);
    }

    bool Value::CanAccept(const Value& value) const
    {
        if (!m_IsNumeric || !value.m_IsNumeric)
            return m_Type == value.m_Type;

        auto it = acceptTable.find(m_Type);

        if (it == acceptTable.end())
            return false;

        auto& acceptedTypes = it->second;

        if (std::ranges::find(acceptedTypes, value.m_Type) == acceptedTypes.end())
            return false;

        return true;
    }

    string Value::GetGlslType(ValueType type)
    {
        switch (type)
        {
        case ValueType::Float32:
            return "float";
        case ValueType::Float32Vector2:
            return "vec2";
        case ValueType::Float32Vector3:
            return "vec3";
        case ValueType::Float32Vector4:
            return "vec4";
        case ValueType::Int32:
            return "int";
        case ValueType::Int32Vector2:
            return "ivec2";
        case ValueType::Int32Vector3:
            return "ivec3";
        case ValueType::Int32Vector4:
            return "ivec4";
        case ValueType::Uint32:
            return "uint";
        case ValueType::Uint32Vector2:
            return "uvec2";
        case ValueType::Uint32Vector3:
            return "uvec3";
        case ValueType::Uint32Vector4:
            return "uvec4";
        case ValueType::Texture2D:
            return "sampler2D";
        }
        return "<<unknown type>>";
    }

    string Value::ToGlslValue()
    {
        switch (m_Type)
        {
        case ValueType::Float32:
            return fmt::format("{0:f}", m_Data.Float32);
        case ValueType::Float32Vector2:
            return fmt::format("vec2({0:f}, {1:f})", m_Data.Float32Vector2.x, m_Data.Float32Vector2.y);
        case ValueType::Float32Vector3:
            return fmt::format("vec3({0}, {1}, {2})", m_Data.Float32Vector3.x, m_Data.Float32Vector3.y,
                               m_Data.Float32Vector3.z);
        case ValueType::Float32Vector4:
            return fmt::format("vec4({0}, {1}, {2}, {3})", m_Data.Float32Vector4.x, m_Data.Float32Vector4.y,
                               m_Data.Float32Vector4.z, m_Data.Float32Vector4.w);
        case ValueType::Int32:
            return fmt::format("{0}", m_Data.Int32);
        case ValueType::Int32Vector2:
            return fmt::format("ivec2({0}, {1})", m_Data.Int32Vector2.x, m_Data.Int32Vector2.y);
        case ValueType::Int32Vector3:
            return fmt::format("ivec3({0}, {1}, {2})", m_Data.Int32Vector3.x, m_Data.Int32Vector3.y,
                               m_Data.Int32Vector3.z);
        case ValueType::Int32Vector4:
            return fmt::format("ivec4({0}, {1}, {2}, {3})", m_Data.Int32Vector4.x, m_Data.Int32Vector4.y,
                               m_Data.Int32Vector4.z, m_Data.Int32Vector4.w);
        case ValueType::Uint32:
            return fmt::format("{0}", m_Data.Uint32);
        case ValueType::Uint32Vector2:
            return fmt::format("uvec2({0}, {1})", m_Data.Uint32Vector2.x, m_Data.Uint32Vector2.y);
        case ValueType::Uint32Vector3:
            return fmt::format("uvec3({0}, {1}, {2})", m_Data.Uint32Vector3.x, m_Data.Uint32Vector3.y,
                               m_Data.Uint32Vector3.z);
        case ValueType::Uint32Vector4:
            return fmt::format("uvec4({0}, {1}, {2}, {3})", m_Data.Uint32Vector4.x, m_Data.Uint32Vector4.y,
                               m_Data.Uint32Vector4.z, m_Data.Uint32Vector4.w);
        case ValueType::Texture2D:
            return "SAMPLER_BINDING";
        default:
            return fmt::format("<<unknown type: {0}>>", static_cast<u32>(m_Type));
        }
    }
}
