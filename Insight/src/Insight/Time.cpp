#include "Time.h"



namespace Insight {
    void Time::Initialize()
    {
        s_StartTime = std::chrono::high_resolution_clock::now();
    }

    float Time::Update()
    {
        auto now = std::chrono::high_resolution_clock::now();

        s_FrameTime = now;
        s_DeltaTime = std::chrono::duration<float>(now - s_StartTime).count();

        return s_DeltaTime;
    }

    float Time::GetDeltaTime()
    {
        return s_DeltaTime;
    }

    float Time::GetFrameTime()
    {
        return std::chrono::duration<float>(s_FrameTime - s_StartTime).count();
    }
} // Insight