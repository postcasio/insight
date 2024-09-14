#pragma once
#include <imgui.h>

namespace Insight
{
    inline int vec2int(const ImVec4 v) { return IM_COL32(v.x * 255, v.y * 255, v.z * 255, v.w * 255); }
}
