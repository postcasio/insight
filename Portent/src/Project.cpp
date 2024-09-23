#include "Project.h"

#include "Insight/Application.h"

namespace Portent {

    Project::Project(const path& path): m_Path(path)
    {
        INS_INFO("Loading project from {0}", path.string());

        m_Directory = path.parent_path();

        json projectData = Fs::ReadJsonFile(path);

        m_Id = Uuid(projectData["Id"].get<string>());
        m_Name = projectData["Name"].get<string>();
        m_LibraryPath = std::filesystem::canonical(m_Directory / projectData["LibraryPath"].get<string>());

        INS_INFO("Project loaded: {0} {1}", m_Name, m_Id);
        INS_INFO("Library path: {0}", m_LibraryPath);

        m_Library.Load(m_LibraryPath);

        m_ThumbnailManager.Initialize(m_Library);

        auto gameModule = m_Directory / "Scripts/Game.js";

        Application::Instance().GetScriptHost().Dispose();
        Application::Instance().GetScriptHost().Initialize();

        if (Fs::FileExists(gameModule))
        {

            if (Application::Instance().GetScriptHost().LoadModule(gameModule))
            {
                INS_ENGINE_INFO("Game module loaded successfully");
            }
            else
            {
                INS_ENGINE_ERROR("Failed to load game module");
            }
        }
    }
}
