#include "Editor.h"
#include <nfd.h>
#include "DemoWindow.h"
#include "DockingHost.h"
#include "ContentBrowser.h"
#include "EditorApplication.h"
#include "Editors/MaterialEditor/MaterialEditor.h"
#include "Editors/PrefabEditor/PrefabEditor.h"
#include "Editors/TextureEditor/TextureEditor.h"
#include "Insight/Library.h"
#include "Wizards/StaticMeshImportWizard.h"

namespace Portent
{
    const AssetEditorFactory g_EditorPanelFactories[] = {
        {AssetType::Texture, []() { return new Editors::TextureEditor(); }},
        {AssetType::Material, []() { return new Editors::MaterialEditor(); }},
    };

    bool TryGetEditorFactory(const AssetType type, std::function<Editors::EditorPanel*()>* outFactory)
    {
        for (const auto& factory : g_EditorPanelFactories)
        {
            if (factory.Type == type)
            {
                *outFactory = factory.Factory;
                return true;
            }
        }

        return false;
    }

    Editor& Editor::Instance()
    {
        return Application::Instance<EditorApplication>().GetEditor();
    }

    Editor::Editor(optional<path> projectPath)
    {
        if (projectPath.has_value())
        {
            m_Project = CreateUnique<Project>(projectPath.value());
        }

        m_Resources = CreateUnique<EditorResources>();
    }

    void Editor::OnAdded()
    {
        RegisterInsightAssetImporters();
        RegisterInsightAssetImportWizards();

        AddPanel(new DockingHost());
        AddPanel(new DemoWindow());
        AddPanel(new ContentBrowser(m_Project != nullptr ? m_Project->GetLibrary().GetDirectory() : "./"));
    }

    void Editor::OpenPath(const path& assetPath)
    {
        AssetDescriptor descriptor;

        if (m_Project->GetLibrary().TryGetAssetDescriptor(assetPath, &descriptor))
        {
            OpenAsset(descriptor);
        }
        else
        {
            INS_CLIENT_ERROR("Asset {0} not found in library!", assetPath.string());
        }
    }

    void Editor::OpenAsset(const AssetDescriptor& descriptor)
    {
        std::function<Editors::EditorPanel*()> factory;

        if (TryGetEditorFactory(descriptor.Type, &factory))
        {
            auto* editor = factory();

            editor->LoadAsset(descriptor.Id);

            AddPanel(editor);
        }
        else
        {
            INS_CLIENT_ERROR("No editor found for asset type {0}", GetAssetTypeName(descriptor.Type));
        }
    }

    void Editor::OnRenderUi()
    {
        Controller::OnRenderUi();

        for (auto& wizard : m_AssetImportWizards)
        {
            wizard->Render();
        }

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::BeginMenu("New"))
                {
                    if (ImGui::MenuItem("Scene"))
                    {
                        auto* editor = new Editors::PrefabEditor();
                        AddPanel(editor);
                    }

                    if (ImGui::MenuItem("Material"))
                    {
                        auto* editor = new Editors::MaterialEditor();
                        AddPanel(editor);
                    }

                    ImGui::EndMenu();
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Exit"))
                {
                    Application::Instance().GetWindow().Close();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::ImportAsset(const path& path)
    {
        for (auto& importer : m_AssetImporters)
        {
            if (importer->IsSupported(path))
            {
                const auto handle = importer->Import(path);

                if (handle != AssetHandle::Null)
                {
                    OpenAsset(GetProject().GetLibrary().GetAssetDescriptor(handle));

                    return;
                }
            }
        }

        for (auto& wizard : m_AssetImportWizards)
        {
            if (wizard->IsSupported(path))
            {
                wizard->Open(path);

                return;
            }
        }
    }

    bool Editor::OpenFileDialog(string* outSelectedPath, const string& defaultPath,
                                const vector<nfdu8filteritem_t>& extensions)
    {
        nfdu8char_t* outPath;
        nfdopendialogu8args_t args;
        args.filterCount = static_cast<uint32_t>(extensions.size());
        args.filterList = extensions.data();
        args.defaultPath = defaultPath.c_str();
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            *outSelectedPath = outPath;
            NFD_FreePathU8(outPath);

            return true;
        }
        else if (result == NFD_CANCEL)
        {
            return false;
        }
        else
        {
            INS_CLIENT_ERROR("Error: {0}", NFD_GetError());
            return false;
        }
    }

    bool Editor::SaveFileDialog(string& outSelectedPath, const string& defaultPath,
                                const vector<nfdu8filteritem_t>& extensions)
    {
        nfdu8char_t* outPath;
        nfdsavedialogu8args_t args;
        args.filterCount = static_cast<uint32_t>(extensions.size());
        args.filterList = extensions.data();
        args.defaultPath = defaultPath.c_str();
        args.defaultName = "Untitled";
        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);

        if (result == NFD_OKAY)
        {
            outSelectedPath = outPath;
            NFD_FreePathU8(outPath);

            return true;
        }
        else if (result == NFD_CANCEL)
        {
            return false;
        }
        else
        {
            INS_CLIENT_ERROR("Error: {0}", NFD_GetError());
            return false;
        }
    }

    void Editor::RegisterAssetImporter(Unique<Importers::AssetImporter> importer)
    {
        m_AssetImporters.push_back(std::move(importer));
    }

    void Editor::RegisterInsightAssetImporters()
    {
        RegisterAssetImporter(CreateUnique<Importers::TextureImporter>());
    }

    void Editor::RegisterAssetImportWizard(Unique<Wizards::AssetImportWizard> wizard)
    {
        m_AssetImportWizards.push_back(std::move(wizard));
    }

    void Editor::RegisterInsightAssetImportWizards()
    {
        RegisterAssetImportWizard(CreateUnique<Wizards::StaticMeshImportWizard>());
    }
}
