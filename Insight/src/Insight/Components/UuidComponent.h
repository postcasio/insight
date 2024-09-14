#pragma once
#include "../Uuid.h"

namespace Insight
{
    struct UuidComponent
    {
        Uuid Id;

        UuidComponent() = default;
        UuidComponent(const UuidComponent&) = default;

        explicit UuidComponent(const Insight::Uuid& id) : Id(id)
        {
        }
    };
}
