#include "TextureEditor.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "TextureEditorProperties.h"
#include "../../Editor.h"
#include "Insight/Library.h"
#include "Insight/Loaders/TextureLoader.h"
#include "Insight/Renderer/Command.h"


namespace Portent::Editors {
    void TextureEditor::OnAdded()
    {
        m_Properties = new TextureEditorProperties(*this);
        AddPanel(m_Properties);
    }

    void TextureEditor::OnRenderContents()
    {
        // ImGui::Text("Texture Viewer");
        if (m_Texture)
        {
            Renderer::Command::SubmitResource(m_Texture->GetImage());
            Renderer::Command::SubmitResource(m_Sampler);

            if (m_RefreshQueued)
            {
                Refresh();
            }

            auto availableSize = ImGui::GetContentRegionAvail();

            availableSize.x -= 20;
            availableSize.y -= 20;

            auto imageSize = m_Texture->GetImage()->GetExtent();

            auto aspect = static_cast<float>(imageSize.x) / static_cast<float>(imageSize.y);

            float width = 0;
            float height = 0;

            if (availableSize.x < availableSize.y)
            {
                width = (availableSize.x);
                height = (width / aspect);
            }
            else
            {
                height = (availableSize.y);
                width = (height * aspect);
            }

            float outerWidth;

            if (m_Texture->GetImage()->GetMipLevels() > 1)
            {
                outerWidth = width * 1.5 + 10;
            }
            else
            {
                outerWidth = width;
            }

            float scaleX = width / static_cast<float>(imageSize.x);
            float scaleY = height / static_cast<float>(imageSize.y);

            auto currentPos = ImGui::GetCursorPos();

            currentPos.x += 10;
            currentPos.y += 10;

            auto offsetX = (availableSize.x - outerWidth) / 2;
            auto offsetY = (availableSize.y - height) / 2;

            ImVec2 position = {currentPos.x + offsetX, currentPos.y + offsetY};
            ImGui::SetCursorPos(position);

            bool shiftAxis = false;
            i32 direction = 1;

            for (u32 i = 0; i < m_Texture->GetImage()->GetMipLevels(); i++)
            {
                auto& mipView = m_MipViews[i];

                Renderer::Command::SubmitResource(mipView.ImageView);

                float mipWidth = mipView.Extent.x * scaleX;
                float mipHeight = mipView.Extent.y * scaleY;


                ImGui::Image(mipView.ImTextureId, ImVec2(mipWidth, mipHeight));

                if (!shiftAxis)
                {
                    position.x += mipWidth * static_cast<float>(direction) + 10;
                }
                else
                {
                    position.y += mipHeight * static_cast<float>(direction) + 10;
                }

                ImGui::SetCursorPos(position);

                shiftAxis = !shiftAxis;

                if (!shiftAxis)
                {
                    // direction *= -1;
                }

            }
        }
    }

    ImGuiID TextureEditor::SetupDockSpace(ImGuiID dockSpaceId)
    {
        ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left, 0.75f, &m_LeftId, &m_RightId);

        ImGui::DockBuilderDockWindow(m_Title.c_str(), m_LeftId);
        ImGui::DockBuilderDockWindow(m_Properties->GetTitle().c_str(), m_RightId);

        return m_LeftId;
    }

    void TextureEditor::LoadAsset(const AssetHandle id)
    {
        auto& library = Editor::Instance().GetProject().GetLibrary();

        m_AssetDescriptor = library.GetAssetDescriptor(id);

        m_TextureJson = Fs::ReadJsonFile(library.GetDirectory() / m_AssetDescriptor.Path);
        m_TexturePath = m_TextureJson["Path"].get<string>();

        m_Texture = library.LoadAsset<Texture>(id, { .CacheData = true });

        m_Filter = m_Texture->GetFilter();
        m_AddressMode = m_Texture->GetAddressMode();
        m_MipLevels = m_Texture->GetImage()->GetMipLevels();
        m_Format = m_Texture->GetFormat();

        m_Title = m_AssetDescriptor.Path.filename().string() + "##Texture Editor " + id.ToString();

        Refresh();
    }

    void TextureEditor::SaveAsset(AssetDescriptor& descriptor)
    {
        const auto& library = Editor::Instance().GetProject().GetLibrary();

        json textureData;

        textureData["Id"] = descriptor.Id.ToString();
        textureData["Path"] = GetTexturePath().lexically_proximate(library.GetDirectory() / descriptor.Path);
        textureData["Filter"] = Loaders::GetFilterName(GetFilter());
        textureData["Format"] = Loaders::GetFormatName(GetFormat());
        textureData["AddressMode"] = Loaders::GetSamplerAddressModeName(GetAddressMode());
        textureData["MipLevels"] = GetMipLevels();

        Fs::WriteJsonFile(library.GetDirectory() / descriptor.Path, textureData);

        INS_CLIENT_INFO("Wrote texture data to {0}", descriptor.Path.c_str());
    }

    void TextureEditor::Refresh()
    {
        m_RefreshQueued = false;

        m_MipViews.clear();
        m_Sampler.reset();

        auto data = m_Texture->GetData();

        auto texData = std::span<u8>(new u8[data.size()], data.size());

        std::ranges::copy(data, texData.begin());

        m_Texture = Texture::Create({
            .Name = m_Texture->GetName(),
            .Data = texData,
            .Width = m_Texture->GetWidth(),
            .Height = m_Texture->GetHeight(),
            .Format = m_Texture->GetFormat(),
            .MipLevels = m_MipLevels,
            .Filter = m_Filter,
            .AddressMode = m_AddressMode,
            .KeepData = true
        });

        m_Sampler = Renderer::Sampler::Create({
            .Name = m_Texture->GetName() + " (Sampler)",
            .MinFilter = m_Filter,
            .MagFilter = m_Filter,
            .AddressModeU = m_AddressMode,
            .AddressModeV = m_AddressMode,
            .AddressModeW = m_AddressMode,
            .MaxLod = static_cast<float>(m_Texture->GetImage()->GetMipLevels())
        });

        uvec2 extent = m_Texture->GetImage()->GetExtent();

        for (u32 i = 0; i < m_Texture->GetImage()->GetMipLevels(); i++)
        {
            auto imageView = Renderer::ImageView::Create({
                .Name = m_Texture->GetName() + " (Mip " + std::to_string(i) + ")",
                .Image = *m_Texture->GetImage(),
                .MipLevels = 1,
                .BaseMipLevel = i
            });

            const auto textureId = Ui::Context::CreateDescriptorSet(*m_Sampler, *imageView);

            m_MipViews.push_back({
                .Extent = extent,
                .ImageView = std::move(imageView),
                .ImTextureId = textureId
            });

            extent = {
                std::max(1u, extent.x / 2),
                std::max(1u, extent.y / 2)
            };
        }
    }

    void TextureEditor::OnRenderMainMenu()
    {
        const bool hasTexture = HasTexture();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, hasTexture))
                {
                    SaveAsset(m_AssetDescriptor);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

    }
}
