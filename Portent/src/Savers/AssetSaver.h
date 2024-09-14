#pragma once

#include "../Portent.h"
#include <Insight/Assets/Asset.h>

#include "../Editors/EditorPanel.h"

namespace Portent::Savers
{
    struct AssetSaveOptions
    {
        AssetDescriptor Descriptor;
    };

    class AssetSaver
    {
    public:
        AssetSaver() = default;
        virtual ~AssetSaver() = default;

        virtual void Save(Editors::EditorPanel* editor, const AssetSaveOptions& options) = 0;
    };
}
