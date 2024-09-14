#pragma once

#include <utility>

#include "../Editor.h"
#include "../Portent.h"
#include "Insight/AssetDescriptor.h"
#include "Insight/Fs.h"
#include "Insight/Uuid.h"

namespace Portent::Controls
{
    struct FileBrowserEntry
    {
        Fs::Entry Entry;

        bool IsAsset = false;
        AssetDescriptor Descriptor;
        AssetType AssetType = AssetType::Unknown;
    };

    struct FileBrowserInfo
    {
        optional<path> RootPath;
    };

    class FileBrowser
    {
    public:
        explicit FileBrowser(const FileBrowserInfo& info = {{}}) : m_RootPath(std::filesystem::canonical(
            info.RootPath.has_value()
                ? info.RootPath.value()
                : std::filesystem::current_path()).string())
        {
            SetPath(m_RootPath);

            Refresh();
        };

        ~FileBrowser() = default;

        bool Render(path* outSelectedPath);
        void Refresh();

        void SetPath(const path& path)
        {
            m_CurrentPath = path;
            m_RelativePath = m_CurrentPath.substr(m_RootPath.length()) + "/";
        }

        [[nodiscard]] const string& GetCurrentPath() const { return m_CurrentPath; }

        [[nodiscard]] const string& GetRelativePath() const { return m_RelativePath; }

    private:
        string m_RootPath;
        string m_CurrentPath;
        string m_RelativePath;
        float m_IconSize = 128;
        float m_Padding = 16;
        bool m_DragDropSource = true;
        bool m_AllFiles = true;

        vector<FileBrowserEntry> m_Entries;

        std::function<void(const path&)> m_OnFileSelected;
    };
}
