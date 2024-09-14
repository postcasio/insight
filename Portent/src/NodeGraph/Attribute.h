#pragma once

#include "GraphTypes.h"
#include "Value.h"

namespace Portent::NodeGraph
{
    struct AttributeInfo
    {
        Id Id;
        string Name;
        Value Value;
        bool Pin = true;
        bool IsModifiable = true;
        bool IsTypeModifiable = false;
        bool IsParameterizable = false;
        bool AcceptsAnyNumeric = false;
        bool Hidden = false;
    };

    class Attribute
    {
    public:
        explicit Attribute(const AttributeInfo &info);
        // Attribute(Attribute &&) noexcept = default;
        // Attribute(const Attribute &) = default;

        [[nodiscard]] Id GetId() const { return m_Id; }
        void SetId(const Id id) { m_Id = id; }
        [[nodiscard]] Value &GetValue() { return m_Value; }
        void SetValue(const Value &value) { m_Value = value; }
        [[nodiscard]] const Value &GetValue() const { return m_Value; }
        [[nodiscard]] const string &GetName() const { return m_Name; }
        [[nodiscard]] bool HasPin() const { return m_Pin; }
        void SetPin(const bool pin) { m_Pin = pin; }
        [[nodiscard]] bool IsModifiable() const { return m_IsModifiable; }
        void SetModifiable(const bool modifiable) { m_IsModifiable = modifiable; }
        [[nodiscard]] bool IsConnected() const { return m_IsConnected; }
        void SetConnected(const bool connected) { m_IsConnected = connected; }
        [[nodiscard]] bool IsParameterizable() const { return m_IsParameterizable; }
        void SetParameterizable(const bool parameterizable) { m_IsParameterizable = parameterizable; }
        [[nodiscard]] bool IsParameterized() const { return m_IsParameterized; }
        void SetParameterized(const bool parameterized) { m_IsParameterized = parameterized; }
        [[nodiscard]] string &GetParameterName() { return m_ParameterName; }
        void SetParameterName(const string &name) { m_ParameterName = name; }
        [[nodiscard]] bool AcceptsAnyNumeric() const { return m_AcceptsAnyNumeric; }
        void SetAcceptsAnyNumeric(const bool acceptsAnyNumeric) { m_AcceptsAnyNumeric = acceptsAnyNumeric; }
        [[nodiscard]] bool IsHidden() const { return m_Hidden; }
        void SetHidden(const bool hidden) { m_Hidden = hidden; }
        [[nodiscard]] bool IsTypeModifiable() const { return m_IsTypeModifiable; }
        void SetTypeModifiable(const bool typeModifiable) { m_IsTypeModifiable = typeModifiable; }

    private:
        Id m_Id;
        Value m_Value;
        string m_Name;
        bool m_Pin;
        bool m_IsModifiable;
        bool m_IsTypeModifiable;
        bool m_IsConnected;
        bool m_IsParameterizable;
        bool m_IsParameterized;
        bool m_AcceptsAnyNumeric;
        string m_ParameterName;
        bool m_Hidden;
    };
}
