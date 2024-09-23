#include "Controller.h"

namespace Insight {
    void Controller::ScriptTrace(JSTracer* tracer)
    {
        for (const auto panel : m_Panels)
        {
            panel->ScriptTrace(tracer);
        }
    }
} // Insight