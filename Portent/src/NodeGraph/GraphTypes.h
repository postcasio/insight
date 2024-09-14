#pragma once

#include "../Portent.h"

namespace Portent::NodeGraph
{
    using Id = i32;

    struct Link
    {
        Id SourceNode;
        Id SourceAttribute;
        Id DestinationNode;
        Id DestinationAttribute;
    };
}