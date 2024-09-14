#include "Entity.h"


std::size_t std::hash<Insight::Entity>::operator()(Insight::Entity const &s) const noexcept
{
    return static_cast<std::size_t>(s.GetHandle());
}