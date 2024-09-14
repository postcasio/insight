#pragma once
#include "AssetLoader.h"
#include "../Renderer/Vertex3D.h"
#include "../Assets/StaticMesh.h"
#include <assimp/scene.h>

namespace Insight::Loaders
{
    class StaticMeshLoader final : public AssetLoader
    {
    public:
        StaticMeshLoader() = default;
        ~StaticMeshLoader() override = default;

        Ref<Asset> Load(const path& staticMeshPath, const AssetLoadOptions& options,
                        std::function<Ref<Asset>(AssetHandle)> loadDependency) override;
        void ProcessAssimpMesh(aiMesh* mesh, vector<Renderer::Vertex3D>& vertices, vector<u32>& indices,
                               vector<StaticMeshSubmesh>& submeshes);
        void ProcessAssimpNode(aiNode* node, const aiScene* scene, vector<Renderer::Vertex3D>& vertices,
                               vector<u32>& indices,
                               vector<StaticMeshSubmesh>& submeshes);
    };
}
