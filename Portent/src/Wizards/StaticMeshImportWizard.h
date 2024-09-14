#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "AssetImportWizard.h"
#include "../Controls/FileBrowser.h"
#include "Insight/Assets/Material.h"
#include "Insight/Assets/StaticMesh.h"
#include "Insight/Renderer/Vertex3D.h"


namespace Portent::Wizards
{
    class StaticMeshImportWizard final : public AssetImportWizard
    {
    public:
        StaticMeshImportWizard() : AssetImportWizard("Static Mesh Importer"), m_FileBrowser({Editor::Instance().GetProject().GetLibrary().GetDirectory()}) {};
        ~StaticMeshImportWizard() override = default;

        StaticMeshSubmesh ProcessAssimpMesh(aiMesh* mesh);
        void ProcessAssimpNode(aiNode* node);
        void OnOpen(const path& assetPath) override;;

        void Reset()
        {
            m_Materials.clear();
            m_Vertices.clear();
            m_Indices.clear();
            m_Submeshes.clear();
        }

        bool IsSupported(const path& path) override;

        void SaveAsset(const AssetDescriptor& descriptor) const;
        void ImportStaticMesh();
        void OnRender() override;

    private:
        path m_FilePath;
        vector<Ref<Material>> m_Materials;
        vector<Renderer::Vertex3D> m_Vertices;
        vector<u32> m_Indices;
        vector<StaticMeshSubmesh> m_Submeshes;
        const aiScene* m_AssimpScene = nullptr;
        Controls::FileBrowser m_FileBrowser;
    };
}
