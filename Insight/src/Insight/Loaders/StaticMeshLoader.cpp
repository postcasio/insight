#include "../Fs.h"

#include "StaticMeshLoader.h"

#include "../Assets/StaticMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace Insight::Loaders
{
    Ref<Asset> StaticMeshLoader::Load(const path& staticMeshPath, const AssetLoadOptions& options,
                                      std::function<Ref<Asset>(AssetHandle)> loadDependency)
    {
        json staticMeshInfo = Fs::ReadJsonFile(staticMeshPath);

        vector<Ref<Material>> materials;

        for (const auto& material : staticMeshInfo["Materials"])
        {
            const auto handle = AssetHandle(material.get<string>());

            materials.push_back(std::static_pointer_cast<Material>(loadDependency(handle)));
        }

        path objectFilePath = std::filesystem::canonical(
            staticMeshPath.parent_path() / (staticMeshInfo["Path"].get<string>()));

        vector<Renderer::Vertex3D> vertices;
        vector<u32> indices;
        vector<StaticMeshSubmesh> submeshes;

        Assimp::Importer importer;

        auto scene = importer.ReadFile(objectFilePath.c_str(), aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_OptimizeMeshes);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            throw std::runtime_error(std::string("Error loading model: ") + importer.GetErrorString());
        }

        ProcessAssimpNode(scene->mRootNode, scene, vertices, indices, submeshes);

        return StaticMesh::Create({
            .Name = staticMeshPath,
            .Vertices = vertices,
            .Indices = indices,
            .Materials = materials,
            .Submeshes = submeshes,
            .KeepData = options.CacheData
        });
    }

    void StaticMeshLoader::ProcessAssimpMesh(aiMesh* mesh, vector<Renderer::Vertex3D>& vertices, vector<u32>& indices, vector<StaticMeshSubmesh>& submeshes)
    {
        StaticMeshSubmesh submesh {
            .BaseIndex = static_cast<u32>(vertices.size()),
            .BaseVertex = static_cast<i32>(indices.size()),
            .MaterialIndex = mesh->mMaterialIndex,
        };

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            Renderer::Vertex3D vertex {
                .Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
                .Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z },
            };

            if (mesh->mTangents)
            {
                vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
            }

            if (mesh->mTextureCoords[0])
            {
                vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
            }

            vertices.push_back(vertex);
        }

        u32 indexCount = 0;

        for (u32 i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            indexCount += face.mNumIndices;

            for (u32 j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        submesh.IndexCount = indexCount;

        submeshes.push_back(submesh);
    }

    void StaticMeshLoader::ProcessAssimpNode(aiNode* node, const aiScene* scene, vector<Renderer::Vertex3D>& vertices, vector<u32>& indices, vector<StaticMeshSubmesh>& submeshes)
    {
        INS_ENGINE_INFO("Processing node {0}", node->mName.C_Str());

        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            ProcessAssimpMesh(mesh, vertices, indices, submeshes);
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessAssimpNode(node->mChildren[i], scene, vertices, indices, submeshes);
        }
    }
}
