#pragma once

#include "../Insight.h"

namespace Insight
{
    struct NameComponent
    {
        string Name;

        NameComponent() = default;
        NameComponent(const NameComponent&) = default;

        explicit NameComponent(const string& name) : Name(name)
        {
        }
    };
}
