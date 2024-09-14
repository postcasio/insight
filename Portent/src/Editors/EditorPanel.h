#pragma once
#include "../Portent.h"
#include "Insight/AssetDescriptor.h"
#include "Insight/Assets/Asset.h"
#include "Insight/Ui/Panel.h"


namespace Portent::Editors
{
    class EditorPanel : public Insight::Panel
    {
    public:
        explicit EditorPanel(const string& name) : Panel(name)
        {
        };

        ~EditorPanel() override = default;

        virtual void LoadAsset(AssetHandle id) = 0;
        virtual void SaveAsset(AssetDescriptor& descriptor) = 0;

        [[nodiscard]] virtual bool HasAsset() const = 0;
        [[nodiscard]] virtual Ref<Asset> GetAsset() const = 0;
        [[nodiscard]] virtual AssetHandle GetAssetHandle() const = 0;

        [[nodiscard]] string GetTitle() const override
        {
            if (HasAsset())
            {
                return fmt::format("{0}##{1}", m_Title, GetAssetHandle());
            }

            return m_Title;
        }
    };
}
