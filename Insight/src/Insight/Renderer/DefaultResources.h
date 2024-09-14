#pragma once

#include "../Insight.h"
#include "../Math.h"
#include "../Renderer/Vulkan.h"

namespace Insight
{
    class Texture;
}

namespace Insight::Renderer
{
    class DefaultResources
    {
    public:
        static void Initialize();
        static void Dispose();

        static Ref<Texture> CreateTexture(vec4 color);
        static Ref<Texture> CreateTexture(const path &texturePath, bool repeat, vk::Format format);

        static Ref<Texture> GetWhiteTexture() { return m_WhiteTexture; }
        static Ref<Texture> GetBlackTexture() { return m_BlackTexture; }
        static Ref<Texture> GetWhiteCubeTexture() { return m_WhiteCubeTexture; }
        static Ref<Texture> GetBlackCubeTexture() { return m_BlackCubeTexture; }
        static Ref<Texture> GetBrdfLut() { return m_BrdfLut; }

    private:
        static inline Ref<Texture> m_WhiteTexture;
        static inline Ref<Texture> m_BlackTexture;
        static inline Ref<Texture> m_WhiteCubeTexture;
        static inline Ref<Texture> m_BlackCubeTexture;
        static inline Ref<Texture> m_BrdfLut;
    };
}
