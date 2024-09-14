#pragma once
#include "../AssetDescriptor.h"

namespace Insight
{

    class Asset
    {
    public:
        Asset() = default;
        explicit Asset(const AssetType type)
        {
            m_Descriptor = {
                .Id = Uuid::CreateNew(),
                .Type = type,
                .Path = ""
            };
        }

        virtual ~Asset() = default;

        void SetHandle(const AssetHandle handle) { m_Handle = handle; }
        [[nodiscard]] AssetHandle GetHandle() const { return m_Handle; }

        void SetDescriptor(const AssetDescriptor& descriptor) { m_Descriptor = descriptor; }
        [[nodiscard]] AssetDescriptor GetDescriptor() const { return m_Descriptor; }

        [[nodiscard]] virtual AssetType GetType() const = 0;
    private:
        AssetHandle m_Handle{};
        AssetDescriptor m_Descriptor{};
    };
}
