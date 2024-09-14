#include "Uuid.h"

namespace Insight {
}

auto fmt::formatter<Insight::Uuid>::format(const Insight::Uuid id, format_context& ctx) const
    -> format_context::iterator {

    return formatter<string_view>::format(id.ToString(), ctx);
}

std::size_t std::hash<Insight::Uuid>::operator()(Insight::Uuid const &s) const noexcept
{
    return std::hash<size_t>{}((
        static_cast<uint64_t>(s.m_A) << 32 ) | s.m_B) ^ std::hash<uint64_t>{}((static_cast<uint64_t>(s.m_C) << 32) | s.m_D
    );
}