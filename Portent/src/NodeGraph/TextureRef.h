#pragma once

#include <utility>

#include "../Portent.h"
#include "Insight/Assets/Texture.h"
#include "../Controls/FileBrowser.h"

namespace Portent::NodeGraph
{
    class TextureRef
    {
    public:
        TextureRef() : m_FileBrowser({ Editor::Instance().GetProject().GetLibrary().GetDirectory() }) {}
        explicit TextureRef(Ref<Texture> texture) : m_Texture(std::move(texture)) {}

        void SetTexture(Ref<Texture> texture) { m_Texture = std::move(texture); }
        [[nodiscard]] Ref<Texture> GetTexture() const { return m_Texture; }
        [[nodiscard]] Controls::FileBrowser& GetFileBrowser() { return m_FileBrowser; }
    private:
        Ref<Texture> m_Texture = nullptr;
        Controls::FileBrowser m_FileBrowser;
    };
}
