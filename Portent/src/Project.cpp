#include "Project.h"

namespace Portent {

    Project::Project(const path& path): m_Path(path)
    {
        INS_CLIENT_INFO("Loading project from {0}", path.string());

        m_Directory = path.parent_path();

        json projectData = Fs::ReadJsonFile(path);

        m_Id = Uuid(projectData["Id"].get<string>());
        m_Name = projectData["Name"].get<string>();
        m_LibraryPath = std::filesystem::canonical(m_Directory / projectData["LibraryPath"].get<string>());

        INS_CLIENT_INFO("Project loaded: {0} {1}", m_Name, m_Id);
        INS_CLIENT_INFO("Library path: {0}", m_LibraryPath);

        m_Library.Load(m_LibraryPath);

        m_ThumbnailManager.Initialize(m_Library);
    }
}
