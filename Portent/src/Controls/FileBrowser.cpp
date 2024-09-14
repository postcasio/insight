#include "FileBrowser.h"

#include "../Editor.h"
#include "../Widgets.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Controls
{
    bool FileBrowser::Render(path* outSelectedPath)
    {
        if (ImGui::Button("Up", ImVec2(20, 0)))
        {
            SetPath(path(m_CurrentPath).parent_path());
            Refresh();
        }

        ImGui::SameLine();

        ImGui::Checkbox("All Files", &m_AllFiles);

        ImGui::SameLine();

        Insight::Widgets::InputText("##Current Path", m_RelativePath);

        bool shouldRefresh = false;
        bool fileSelected = false;

        ImVec2 outerSize = ImGui::GetContentRegionAvail();

        ImVec2 innerSize = ImVec2(outerSize.x - m_Padding * 2, outerSize.y - m_Padding * 2);

        float columnWidth = m_IconSize;

        int columns = (int)(innerSize.x / columnWidth);
        if (columns < 1)
        {
            columns = 1;
        }

        columnWidth = innerSize.x / columns;

        int currentColumn = 0;

        if (ImGui::BeginTable("##Files", columns, ImGuiTableFlags_None))
        {
            for (auto& entry : m_Entries)
            {
                if (!m_AllFiles && (!entry.IsAsset && entry.AssetType != AssetType::Directory))
                {
                    continue;
                }

                if (currentColumn == 0)
                {
                    ImGui::TableNextRow();
                }

                ImGui::TableNextColumn();


                const auto position = ImGui::GetCursorScreenPos();
                const auto positionIconEnd = ImVec2(position.x + columnWidth, position.y + m_IconSize + m_Padding / 2);
                const auto positionEnd = ImVec2(position.x + columnWidth, position.y + m_IconSize + m_Padding / 2 + ImGui::GetTextLineHeightWithSpacing());

                const bool hovered = ImGui::IsMouseHoveringRect(position, positionEnd);

                ImGui::GetWindowDrawList()->AddRectFilled(
                    position,
                    positionEnd,
                        hovered
                            ? static_cast<ImU32>(ImColor(ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered)))
                            : IM_COL32(0, 0, 0, 0),
                    0.0f
                );

                ImGui::BeginGroup();
                ImGui::PushID(entry.Descriptor.Id.ToString().c_str());

                ImGui::SetCursorScreenPos(ImVec2(position.x + (columnWidth - m_IconSize) / 2, position.y + m_Padding / 2));

                if (entry.IsAsset)
                {
                    Widgets::AssetPreview(entry.Descriptor, ImVec2(m_IconSize, m_IconSize));
                }
                else
                {
                    Widgets::AssetIcon(entry.AssetType, ImVec2(m_IconSize, m_IconSize));
                }

                ImGui::Text("%s", entry.Entry.Name.c_str());

                ImGui::PopID();
                ImGui::EndGroup();

                if (entry.IsAsset)
                {
                    Widgets::AssetDragDropSource(entry.Descriptor);
                }

                if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && hovered)
                {
                    if (entry.Entry.IsDirectory)
                    {
                        SetPath(entry.Entry.Path);

                        shouldRefresh = true;
                    }
                    else
                    {
                        outSelectedPath->assign(entry.Entry.Path);

                        fileSelected = true;
                    }
                }

                currentColumn = (currentColumn + 1) % columns;
            }

            ImGui::EndTable();
        }

        if (shouldRefresh)
        {
            Refresh();
        }

        return fileSelected;
    }

    void FileBrowser::Refresh()
    {
        auto& library = Editor::Instance().GetProject().GetLibrary();

        m_Entries.clear();

        for (const auto& entry : Fs::GetDirectoryContents(m_CurrentPath))
        {
            AssetDescriptor descriptor;

            if (library.TryGetAssetDescriptor(entry.Path, &descriptor))
            {
                m_Entries.push_back({
                    entry,
                    true,
                    descriptor,
                    descriptor.Type
                });
            }
            else
            {
                m_Entries.push_back({
                    entry,
                    false,
                    descriptor,
                    entry.IsDirectory ? AssetType::Directory : AssetType::Unknown
                });
            }
        }

        // sort by filename

        std::sort(m_Entries.begin(), m_Entries.end(), [](const FileBrowserEntry& a, const FileBrowserEntry& b) {
            return a.Entry.Name < b.Entry.Name;
        });
    }
}
