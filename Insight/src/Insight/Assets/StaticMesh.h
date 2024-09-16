#pragma once

#include <utility>

#include "Material.h"
#include "../Renderer/Buffer.h"
#include "../Renderer/Vertex3D.h"
#include "../Renderer/CommandBuffer.h"

namespace Insight
{
    struct StaticMeshSubmesh
    {
        i32 BaseVertex;
        u32 BaseIndex;
        u32 IndexCount;
        u32 MaterialIndex;
    };

    struct StaticMeshInfo
    {
        string Name;
        std::span<Renderer::Vertex3D> Vertices;
        std::span<u32> Indices;
        vector<StaticMeshSubmesh> Submeshes;
        vector<Ref<Material>> Materials{};
        bool KeepData;
    };

    class StaticMesh final : public Asset
    {
    public:
        static Ref<StaticMesh> Create(const StaticMeshInfo& info)
        {
            return CreateRef<StaticMesh>(info);
        }

        explicit StaticMesh(const StaticMeshInfo& info);
        ~StaticMesh() override = default;

        [[nodiscard]] const string& GetName() const
        {
            return m_Name;
        }

        [[nodiscard]] const Ref<Renderer::Buffer>& GetVertexBuffer() const
        {
            return m_VertexBuffer;
        }

        [[nodiscard]] const Ref<Renderer::Buffer>& GetIndexBuffer() const
        {
            return m_IndexBuffer;
        }

        [[nodiscard]] const vector<StaticMeshSubmesh>& GetSubmeshes() const
        {
            return m_Submeshes;
        }

        [[nodiscard]] const vector<Ref<Material>>& GetMaterials() const
        {
            return m_Materials;
        }

        Ref<Material> GetMaterial(const u32 material_index)
        {
            return m_Materials[material_index];
        }

        void SetMaterial(int i, Ref<Material> material)
        {
            m_Materials[i] = std::move(material);
        }

        [[nodiscard]] AssetType GetType() const override { return AssetType::StaticMesh; }
        void BindBuffers(const Renderer::CommandBuffer& commandBuffer) const;
        void DrawAllSubmeshes(const Renderer::CommandBuffer& commandBuffer) const;

    private:
        string m_Name{};

        Ref<Renderer::Buffer> m_VertexBuffer{};
        Ref<Renderer::Buffer> m_IndexBuffer{};

        vector<StaticMeshSubmesh> m_Submeshes{};
        vector<Ref<Material>> m_Materials{};
    };
} // Insight
