#pragma once

#include "../Portent.h"
#include "./AssetSaver.h"
#include <Insight/AssetDescriptor.h>

namespace Portent::Savers {
    class TextureSaver : public AssetSaver
    {
    public:
        ~TextureSaver() override = default;
        void Save(Editors::EditorPanel* editor, const AssetSaveOptions& options) override;
    };

}
