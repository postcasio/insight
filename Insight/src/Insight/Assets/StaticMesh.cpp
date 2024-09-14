#include "StaticMesh.h"

#include "../Renderer/Vertex3D.h"

namespace Insight {
    StaticMesh::StaticMesh(const StaticMeshInfo& info) : Asset(AssetType::StaticMesh), m_Name(info.Name), m_Submeshes(info.Submeshes), m_Materials(info.Materials)
    {
        m_VertexBuffer = Renderer::Buffer::Create({
            .Name = info.Name + " VertexBuffer",
            .Size = info.Vertices.size_bytes(),
            .Usage = vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst
        });

        m_VertexBuffer->Upload(std::span(reinterpret_cast<u8*>(info.Vertices.data()), info.Vertices.size_bytes()));

        m_IndexBuffer = Renderer::Buffer::Create({
            .Name = info.Name + " IndexBuffer",
            .Size = info.Indices.size_bytes(),
            .Usage = vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst
        });

        m_IndexBuffer->Upload(std::span(reinterpret_cast<u8*>(info.Indices.data()), info.Indices.size_bytes()));
    }

    void StaticMesh::BindBuffers(const Renderer::CommandBuffer& commandBuffer) const
    {
        commandBuffer.BindVertexBuffer(*m_VertexBuffer);
        commandBuffer.BindIndexBuffer(*m_IndexBuffer);
    }

    void StaticMesh::DrawAllSubmeshes(const Renderer::CommandBuffer& commandBuffer) const
    {
        for (const auto& submesh : m_Submeshes)
        {
            commandBuffer.DrawIndexed(submesh.IndexCount, 1, submesh.BaseIndex, submesh.BaseVertex, 0);
        }
    }
} // Insight