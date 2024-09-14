#pragma once
#include "Editors/EditorPanel.h"
#include "Controls/FileBrowser.h"


namespace Portent {

class ContentBrowser final : public Panel {
public:
    explicit ContentBrowser(const optional<path>& rootPath);;
    ~ContentBrowser() override = default;

    void OnRenderContents() override;
private:
    Controls::FileBrowser m_FileBrowser;
};

}
