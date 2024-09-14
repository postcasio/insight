#pragma once

#include <chrono>

namespace Insight {

    using time_point = std::chrono::high_resolution_clock::time_point;

class Time {
public:
    static void Initialize();
    static float Update();

    static float GetDeltaTime();
    static float GetFrameTime();
private:
    static inline float s_DeltaTime = 0.0f;
    static inline time_point s_FrameTime{};
    static inline time_point s_StartTime{};
};

} // Insight
