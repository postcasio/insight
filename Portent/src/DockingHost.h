#pragma once
#include "Insight/Ui/Panel.h"

namespace Portent
{
    class DockingHost final : public Insight::Panel
    {
    public:
        DockingHost() : Panel("DockingHost") {};
        ~DockingHost() override = default;

        void OnRender() override;
    };
}
