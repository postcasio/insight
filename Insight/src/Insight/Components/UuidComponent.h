#pragma once
#include "../Uuid.h"

namespace Insight
{
    class Entity;

    struct UuidComponent
    {
        static inline const Uuid ComponentId = Uuid {"8f5f65b1-c420-46ff-b3bf-df178569f377"};
        static inline const string ComponentName = "UuidComponent";
        static void AddFunction(Entity* entity);

        Uuid Id;

        UuidComponent() = default;
        UuidComponent(const UuidComponent&) = default;

        explicit UuidComponent(const Insight::Uuid& id) : Id(id)
        {
        }
    };
}
