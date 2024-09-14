#include "Widgets.h"
#include "Insight/Ui/Widgets.h"

#include <imgui_internal.h>

#include "Controls/FileBrowser.h"
#include "Insight/Ui/Utils.h"

namespace Portent
{
    bool Widgets::AssetSelector(const string& label, const Ref<Asset>& asset, const AssetType assetType, const ImVec2& size,
                                float brickWidth, Controls::FileBrowser& fileBrowser, AssetDescriptor* outSelectedAsset)
    {
        bool fileSelected = false;

        AssetBrick(asset, assetType, size, brickWidth);

        if (AssetDragDropTarget(assetType, outSelectedAsset))
        {
            fileSelected = true;
        }

        const auto popupName = "Browser##" + label;

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            ImGui::OpenPopup(popupName.c_str());
        }

        ImGui::SetNextWindowSize({300, 200});
        if (ImGui::BeginPopup(popupName.c_str(), ImGuiWindowFlags_AlwaysAutoResize))
        {
            path selectedPath;
            if (fileBrowser.Render(&selectedPath))
            {
                fileSelected = Editor::Instance().GetProject().GetLibrary().TryGetAssetDescriptor(selectedPath, outSelectedAsset);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return fileSelected;
    }

    bool Widgets::PropertyTreeAssetSelector(const string& label, const Ref<Asset>& asset,
                                            const AssetType assetType, Controls::FileBrowser& fileBrowser, AssetDescriptor* outSelectedAsset)
    {
        Insight::Widgets::PropertyTreeLabel(label);
        ImGui::TableSetColumnIndex(1);
        const auto avail = ImGui::GetContentRegionAvail();
        ImGui::SetNextItemWidth(avail.x);

        return AssetSelector(label, asset, assetType, {128, 128}, -1, fileBrowser, outSelectedAsset);
    }

    bool Widgets::AssetDragDropTarget(const AssetType type, AssetDescriptor* outSelectedAsset)
    {
        bool accepted = false;

        if (ImGui::BeginDragDropTarget())
        {
            const auto payload = ImGui::AcceptDragDropPayload(GetAssetTypeName(type).c_str());

            if (payload)
            {
                const AssetHandle handle = *static_cast<AssetHandle*>(payload->Data);

                *outSelectedAsset = Editor::Instance().GetProject().GetLibrary().GetAssetDescriptor(handle);

                accepted = true;
            }

            ImGui::EndDragDropTarget();
        }

        return accepted;
    }

    void Widgets::AssetDragDropSource(const AssetDescriptor& descriptor)
    {
        constexpr auto iconSize = 32.0f;

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload(GetAssetTypeName(descriptor.Type).c_str(), &descriptor.Id, sizeof(AssetHandle));

            const auto position = ImGui::GetCursorPos();

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {8, 8});

            if (ImGui::BeginTable("##Asset", 2, ImGuiTableFlags_BordersOuterV))
            {
                ImGui::TableSetupColumn("##Icon", ImGuiTableColumnFlags_WidthFixed, iconSize);
                ImGui::TableSetupColumn("##Text", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();

                ImGui::TableNextColumn();

                AssetPreview(descriptor, {iconSize, iconSize});

                ImGui::TableNextColumn();

                ImGui::TextDisabled("%s", GetAssetTypeName(descriptor.Type).c_str());
                ImGui::Text("%s", descriptor.Path.filename().c_str());

                ImGui::EndTable();
            }

            ImGui::PopStyleVar();

            ImGui::EndDragDropSource();
        }
    }

    void Widgets::AssetIconCenteredScreenPos(const AssetType type, const ImVec2& position, const ImVec2& size)
    {
        Insight::Widgets::ImageCenteredScreenPos(Editor::Instance().GetResources().GetAssetTypeIcon(type)->GetImage(), position, size);
    }

    void Widgets::AssetIcon(const AssetType type, const ImVec2& size)
    {
        Insight::Widgets::Image(Editor::Instance().GetResources().GetAssetTypeIcon(type)->GetImage(), size);
    }

    void Widgets::AssetThumbnail(const AssetDescriptor& descriptor, const ImVec2& size)
    {
        auto& thumbnailManager = Editor::Instance().GetProject().GetThumbnailManager();

        const auto thumbnail = thumbnailManager.GetThumbnailTexture(descriptor.Id);

        if (thumbnail != nullptr)
        {
            Insight::Widgets::Image(thumbnail->GetImage(), size);
        }
        else
        {
            AssetIcon(descriptor.Type, size);
        }
    }

    void Widgets::AssetPreview(const AssetDescriptor& descriptor, const ImVec2& size)
    {
        const auto& thumbnailManager = Editor::Instance().GetProject().GetThumbnailManager();

        if (thumbnailManager.CanGenerateThumbnail(descriptor.Type))
        {
            AssetThumbnail(descriptor, size);
        }
        else
        {
            AssetIcon(descriptor.Type, size);
        }
    }

    void Widgets::AssetBrick(const Ref<Asset>& asset, const AssetType assetType, const ImVec2& size, float brickWidth)
    {
        const auto pos = ImGui::GetCursorScreenPos();
        const auto colors = ImGui::GetStyle().Colors;
        const auto windowPos = ImGui::GetWindowPos();
        const auto windowSize = ImGui::GetWindowSize();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {4, 4});

        if (ImGui::BeginTable("##Asset", 2, ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersOuterH, ImVec2(brickWidth, 0)))
        {
            ImGui::TableSetupColumn("##Icon", ImGuiTableColumnFlags_WidthFixed, size.x);
            ImGui::TableSetupColumn("##Text", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(colors[ImGuiCol_WindowBg]));

            if (asset)
            {
                const auto descriptor = asset->GetDescriptor();

                AssetPreview(descriptor, size);

                ImGui::TableNextColumn();
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(colors[ImGuiCol_WindowBg]));
                ImGui::TextDisabled("%s", GetAssetTypeName(descriptor.Type).c_str());
                ImGui::Text("%s", descriptor.Path.filename().c_str());
            }
            else
            {
                AssetIcon(assetType, size);

                ImGui::TableNextColumn();
                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, ImGui::GetColorU32(colors[ImGuiCol_WindowBg]));
                ImGui::TextDisabled("%s", GetAssetTypeName(assetType).c_str());
            }

            ImGui::EndTable();
        }

        ImGui::PopStyleVar();
    }
} // Portent
