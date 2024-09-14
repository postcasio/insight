#include "ContentBrowser.h"

#include <imgui.h>

#include "Editor.h"


namespace Portent
{
    ContentBrowser::ContentBrowser(const optional<path>& rootPath): Panel("Content Browser"),
                                                                   m_FileBrowser({
                                                                       .RootPath = rootPath
                                                                   })
    {
    }

    void ContentBrowser::OnRenderContents()
    {
        path selectedPath;
        AssetDescriptor selectedAsset;

        if (m_FileBrowser.Render(&selectedPath))
        {
            if (Editor::Instance().GetProject().GetLibrary().TryGetAssetDescriptor(selectedPath, &selectedAsset))
            {
                Editor::Instance().OpenAsset(selectedAsset);
            }
            else
            {
                Editor::Instance().ImportAsset(selectedPath);
            }
        }
    }

}
