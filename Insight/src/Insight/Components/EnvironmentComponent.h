#pragma once

#include "../Insight.h"
#include "../Environment.h"

namespace Insight
{
    struct EnvironmentComponent
    {
        Ref<Environment> Environment = CreateRef<Insight::Environment>();

        EnvironmentComponent() = default;
        ~EnvironmentComponent() = default;
        EnvironmentComponent(const EnvironmentComponent&) = default;
    };
}
