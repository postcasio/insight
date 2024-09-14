#include "Attribute.h"

namespace Portent::NodeGraph {
    Attribute::Attribute(const AttributeInfo& info) : m_Id(info.Id), m_Value(info.Value), m_Name(info.Name), m_Pin(info.Pin), m_IsModifiable(info.IsModifiable), m_IsConnected(false), m_IsParameterizable(info.IsParameterizable), m_IsParameterized(false), m_ParameterName(info.Name), m_AcceptsAnyNumeric(info.AcceptsAnyNumeric), m_Hidden(info.Hidden), m_IsTypeModifiable(info.IsTypeModifiable)
    {

    }

}
