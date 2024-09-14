#pragma once

#include "Portent.h"
#include "Insight/Ui/Panel.h"

namespace Portent {
class DemoWindow : public Panel {
public:
    DemoWindow() : Panel("Demo Window") {};
    ~DemoWindow() override = default;

    void OnRender() override;
};
}
