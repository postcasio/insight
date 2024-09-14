#pragma once
#include "Asset.h"

namespace Insight
{
    struct PrefabInfo
    {
        string Name;
    };

    class Prefab final : public Asset
    {
    public:
        explicit Prefab(const PrefabInfo& info) : m_Name(info.Name)
        {
        }

        ~Prefab() override = default;

        [[nodiscard]] AssetType GetType() const override { return AssetType::Prefab; }
        [[nodiscard]] const string& GetName() const { return m_Name; }

    private:
        string m_Name;
    };
} // Insight
