#include "StaticMeshImportWizard.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Editor.h"
#include "../Widgets.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Wizards
{
    StaticMeshSubmesh StaticMeshImportWizard::ProcessAssimpMesh(aiMesh* mesh)
    {
        StaticMeshSubmesh submesh {
            .BaseIndex = static_cast<u32>(m_Vertices.size()),
            .BaseVertex = static_cast<i32>(m_Indices.size()),
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

            m_Vertices.push_back(vertex);
        }

        u32 indexCount = 0;

        for (u32 i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            indexCount += face.mNumIndices;

            for (u32 j = 0; j < face.mNumIndices; j++)
            {
                m_Indices.push_back(face.mIndices[j]);
            }
        }

        submesh.IndexCount = indexCount;

        return submesh;
    }

    void StaticMeshImportWizard::ProcessAssimpNode(aiNode* node)
    {
        std::string name = std::string(node->mName.C_Str());

        INS_INFO("Processing node {0}", name);

        for (uint32_t i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = m_AssimpScene->mMeshes[node->mMeshes[i]];

            m_Submeshes.push_back(ProcessAssimpMesh(mesh));
        }

        for (uint32_t i = 0; i < node->mNumChildren; i++)
        {
            ProcessAssimpNode(node->mChildren[i]);
        }
    }

    void StaticMeshImportWizard::OnOpen(const path& assetPath)
    {
        INS_INFO("Importing static mesh from {0}", assetPath.string());

        Reset();

        m_FilePath = assetPath;

        Assimp::Importer importer;

        m_AssimpScene = importer.ReadFile(m_FilePath.c_str(), aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_OptimizeMeshes);

        if (!m_AssimpScene || m_AssimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_AssimpScene->mRootNode)
        {
            throw std::runtime_error(std::string("Error loading model: ") + importer.GetErrorString());
        }

        ProcessAssimpNode(m_AssimpScene->mRootNode);

        u32 largestMaterialIndex = 0;
        for (const auto& submesh : m_Submeshes)
        {
            if (submesh.MaterialIndex > largestMaterialIndex)
            {
                largestMaterialIndex = submesh.MaterialIndex;
            }
        }

        m_Materials.resize(largestMaterialIndex + 1);
    }

    bool StaticMeshImportWizard::IsSupported(const path& path)
    {
        auto extension = path.extension().string();

        std::ranges::transform(extension, extension.begin(), ::tolower);

        auto supportedExtensions = { ".obj", ".gltf", ".fbx" };

        return std::ranges::find(supportedExtensions, extension) != supportedExtensions.end();
    }

    void StaticMeshImportWizard::SaveAsset(const AssetDescriptor& descriptor) const
    {
        auto& library = Editor::Instance().GetProject().GetLibrary();

        json assetData;

        assetData["Id"] = descriptor.Id.ToString();
        assetData["Path"] = m_FilePath.lexically_proximate(library.GetDirectory() / descriptor.Path.parent_path());
        assetData["Submeshes"] = json::array();
        for (const auto& submesh : m_Submeshes)
        {
            assetData["Submeshes"].push_back({
                {"BaseIndex", submesh.BaseIndex},
                {"BaseVertex", submesh.BaseVertex},
                {"IndexCount", submesh.IndexCount},
                {"MaterialIndex", submesh.MaterialIndex},
            });
        }
        assetData["Materials"] = json::array();
        for (const auto& material : m_Materials)
        {
            if (material)
            {
                assetData["Materials"].push_back(material->GetHandle().ToString());
            }
            else
            {
                assetData["Materials"].push_back(nullptr);
            }
        }

        Fs::WriteJsonFile(library.GetDirectory() / descriptor.Path, assetData);
    }

    void StaticMeshImportWizard::ImportStaticMesh()
    {
        string newPath;
        auto& library = Editor::Instance().GetProject().GetLibrary();

        if (Editor::SaveFileDialog(newPath, Editor::Instance().GetProject().GetLibrary().GetDirectory(),
                                   {{"StaticMesh", "InsightStaticMesh"}}))
        {
            AssetDescriptor descriptor;

            if (library.TryGetAssetDescriptor(newPath, &descriptor))
            {
                library.RemoveAsset(descriptor.Id);
            }

            descriptor = {
                .Path = path(newPath).lexically_proximate(Editor::Instance().GetProject().GetLibrary().GetDirectory()),
                .Type = AssetType::StaticMesh,
                .Id = AssetHandle::CreateNew()
            };

            SaveAsset(descriptor);
            library.AddAsset(descriptor);
        }
    }

    void StaticMeshImportWizard::OnRender()
    {
        if (Insight::Widgets::BeginPropertyTree("Mesh", {-1, 300}))
        {
            Insight::Widgets::PropertyTreeText("Path", m_FilePath.lexically_proximate(Editor::Instance().GetProject().GetLibrary().GetDirectory()));
            Insight::Widgets::PropertyTreeText("Vertices", fmt::format("{0} ({1})", m_Vertices.size(), Insight::bytesize(m_Vertices.size()* sizeof(Renderer::Vertex3D)).format()));
            Insight::Widgets::PropertyTreeText("Indices", fmt::format("{0} ({1})", m_Indices.size(), Insight::bytesize(m_Indices.size() * sizeof(u32)).format()));
            if (Insight::Widgets::ExpandingPropertyTreeLabel(fmt::format("Submeshes ({0})", m_Submeshes.size())))
            {
                for (size_t i = 0; i < m_Submeshes.size(); i++)
                {
                    auto& submesh = m_Submeshes[i];

                    if (Insight::Widgets::ExpandingPropertyTreeLabel(fmt::format("Submesh {0}", i)))
                    {
                        Insight::Widgets::PropertyTreeText("Base Index", fmt::format("{0}", submesh.BaseIndex));
                        Insight::Widgets::PropertyTreeText("Base Vertex", fmt::format("{0}", submesh.BaseVertex));
                        Insight::Widgets::PropertyTreeText("Index Count", fmt::format("{0}", submesh.IndexCount));
                        Insight::Widgets::PropertyTreeText("Material Index", fmt::format("{0}", submesh.MaterialIndex));
                    }
                }
            }
            if (Insight::Widgets::ExpandingPropertyTreeLabel(fmt::format("Materials ({0})", m_Materials.size())))
            {
                for (size_t i = 0; i < m_Materials.size(); i++)
                {
                    AssetDescriptor descriptor;
                    if (Widgets::PropertyTreeAssetSelector(fmt::format("Material {0}", i), m_Materials[i], AssetType::Material, m_FileBrowser, &descriptor))
                    {
                        if (descriptor.Type == AssetType::Material)
                        {
                            m_Materials[i] = Editor::Instance().GetProject().GetLibrary().LoadAsset<Material>(descriptor.Id, {
                                .CacheData = false
                            });
                        }
                    }
                }
            }

            Insight::Widgets::EndPropertyTree();
        }

        if (Insight::Widgets::Button("Import As..."))
        {
            ImportStaticMesh();
        }

    }
}
