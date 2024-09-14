#pragma once

#include "Insight.h"
#include "Assets/Texture.h"

namespace Insight
{
    struct CreateTextureInfo
    {
        string Name;
        u32 Width;
        u32 Height;
        vec4 ClearColor;
    };

    Ref<Texture> CreateTexture(const CreateTextureInfo& info);
}
