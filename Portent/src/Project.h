#pragma once

#include "Portent.h"
#include "Insight/Fs.h"
#include "Insight/Library.h"
#include "Insight/Uuid.h"
#include "EditorLibrary.h"
#include "Insight/Scripting/ScriptHost.h"
#include "Thumbnails/ThumbnailManager.h"

namespace Portent
{
    class Project
    {
    public:
        explicit Project(const path& path);
        void LoadEntities();
        ~Project() = default;

        [[nodiscard]] EditorLibrary& GetLibrary() { return m_Library; }
        [[nodiscard]] Thumbnails::ThumbnailManager& GetThumbnailManager() { return m_ThumbnailManager; }
        [[nodiscard]] const path& GetPath() const { return m_Path; }
        [[nodiscard]] const path& GetDirectory() const { return m_Directory; }
        [[nodiscard]] const string& GetName() const { return m_Name; }
        [[nodiscard]] const Uuid& GetId() const { return m_Id; }
        [[nodiscard]] const string& GetLibraryPath() const { return m_LibraryPath; }

    private:
        Uuid m_Id;

        path m_Path;
        path m_Directory;
        string m_LibraryPath;
        string m_Name;

        EditorLibrary m_Library;
        Thumbnails::ThumbnailManager m_ThumbnailManager;

    };
}
