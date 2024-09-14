#include "TextureEditorProperties.h"

#include "Insight/Ui/Widgets.h"

namespace Portent::Editors {
    static const Widgets::ComboOption<vk::Filter> s_TextureEditorFilterOptions[] = {
        { vk::Filter::eLinear, "Linear" },
        { vk::Filter::eNearest, "Nearest" }
    };

    static constexpr u32 s_TextureEditorFilterOptionCount = sizeof(s_TextureEditorFilterOptions) / sizeof(Widgets::ComboOption<vk::Filter>);

    static const Widgets::ComboOption<vk::Format> s_TextureEditorFormatOptions[] = {
        {vk::Format::eR8G8B8A8Unorm, "Rgba8Linear"},
        {vk::Format::eR8G8B8A8Srgb, "Rgba8Srgb"},
        {vk::Format::eR16G16B16A16Sfloat, "Rgba16Float"},
        {vk::Format::eR16G16B16Sfloat, "Rgb16Float"},
        {vk::Format::eR32G32B32A32Sfloat, "Rgba32Float"},
    };

    static constexpr u32 s_TextureEditorFormatOptionCount = sizeof(s_TextureEditorFormatOptions) / sizeof(Widgets::ComboOption<vk::Format>);

    static const Widgets::ComboOption<vk::SamplerAddressMode> s_TextureEditorAddressModeOptions[] = {
        { vk::SamplerAddressMode::eRepeat, "Repeat" },
        { vk::SamplerAddressMode::eMirroredRepeat, "Mirrored Repeat" },
        { vk::SamplerAddressMode::eClampToEdge, "Clamp to Edge" },
        { vk::SamplerAddressMode::eClampToBorder, "Clamp to Border" },
        { vk::SamplerAddressMode::eMirrorClampToEdge, "Mirror Clamp to Edge" }
    };

    static constexpr u32 s_TextureEditorAddressModeOptionCount = sizeof(s_TextureEditorAddressModeOptions) / sizeof(Widgets::ComboOption<vk::SamplerAddressMode>);

    void TextureEditorProperties::OnRenderContents()
    {
        if (m_TextureEditor.HasTexture())
        {
            const auto& texture = m_TextureEditor.GetTexture();

            const auto size = ImGui::GetContentRegionAvail();

            if (Widgets::BeginPropertyTree("Texture Properties", size))
            {
                auto assetId = texture->GetHandle().ToString();

                Widgets::PropertyTreeText("ID", assetId);

                if (Widgets::PropertyTreeCombo("Filter", m_TextureEditor.GetFilter(), s_TextureEditorFilterOptions, s_TextureEditorFilterOptionCount))
                {
                    m_TextureEditor.QueueRefresh();
                }

                if (Widgets::PropertyTreeCombo("Format", m_TextureEditor.GetFormat(), s_TextureEditorFormatOptions, s_TextureEditorFormatOptionCount))
                {
                    m_TextureEditor.QueueRefresh();
                }

                if (Widgets::PropertyTreeCombo("Address Mode", m_TextureEditor.GetAddressMode(), s_TextureEditorAddressModeOptions, s_TextureEditorAddressModeOptionCount))
                {
                    m_TextureEditor.QueueRefresh();
                }

                if (Widgets::PropertyTreeU32("Mip Levels", m_TextureEditor.GetMipLevels(), 1, 1, texture->GetMaximumSupportedMipLevels()))
                {
                    m_TextureEditor.QueueRefresh();
                }

                Widgets::EndPropertyTree();
            }
        }
    }
}
