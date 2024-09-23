#pragma once

#include <functional>
#include <iosfwd>

#include "EditorResources.h"
#include "nfd.h"
#include "Portent.h"
#include "Project.h"
#include "Editors/EditorPanel.h"
#include "Insight/Application.h"
#include "Insight/Controller.h"
#include "Wizards/AssetImportWizard.h"

namespace Portent
{
    struct AssetEditorFactory
    {
        AssetType Type = AssetType::Unknown;
        std::function<Editors::EditorPanel*()> Factory;
    };

    extern const AssetEditorFactory g_EditorPanelFactories[];

    bool TryGetEditorFactory(AssetType type, std::function<Editors::EditorPanel*()>* outFactory);

    class Editor final : public Insight::Controller
    {
    public:
        static Editor& Instance();

        explicit Editor(optional<path> projectPath);
        ~Editor() override = default;

        void OnAdded() override;
        void OpenPath(const path& path);
        void OpenAsset(const AssetDescriptor& descriptor);
        void OnRenderUi() override;

        [[nodiscard]] const EditorResources& GetResources() const { return *m_Resources; }

        [[nodiscard]] Project& GetProject () const { return *m_Project; }
        void ImportAsset(const path& path);

        [[nodiscard]] static bool OpenFileDialog(string* outSelectedPath, const string& defaultPath, const vector<nfdu8filteritem_t>& extensions = {});
        [[nodiscard]] static bool SaveFileDialog(string& outSelectedPath, const string& defaultPath, const vector<nfdu8filteritem_t>& extensions = {});

        void RegisterAssetImporter(Unique<Importers::AssetImporter> importer);
        void RegisterInsightAssetImporters();

        void RegisterAssetImportWizard(Unique<Wizards::AssetImportWizard> wizard);
        void RegisterInsightAssetImportWizards();

    private:
        Unique<Project> m_Project = nullptr;
        Unique<EditorResources> m_Resources = nullptr;
        vector<Unique<Importers::AssetImporter>> m_AssetImporters{};
        vector<Unique<Wizards::AssetImportWizard>> m_AssetImportWizards{};

    };
} // Portent
