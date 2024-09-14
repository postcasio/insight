#include "MaterialLoader.h"
#include "../Fs.h"
#include "../Base64.h"
#include "../Assets/Material.h"

namespace Insight::Loaders
{
    Ref<Asset> MaterialLoader::Load(const path& materialPath, const AssetLoadOptions& options, std::function<Ref<Asset>(AssetHandle)> loadDependency)
    {
        json materialInfo = Fs::ReadJsonFile(materialPath);
        string shaderEncoded = materialInfo["Shader"];

        auto shaderBinary = Base64::decode_into<vector<u8>>(shaderEncoded);

        vector<Ref<Texture>> textures{};

        for (const auto& texture : materialInfo["Textures"])
        {
            const auto handle = AssetHandle(texture.get<string>());
            textures.push_back(std::static_pointer_cast<Texture>(loadDependency(handle)));
        }

        vector<MaterialUniformInfo> uniforms{};

        for (const auto& uniform : materialInfo["Uniforms"])
        {
            const MaterialUniformInfo info = {
                .Name = uniform["Name"].get<string>(),
                .Size = uniform["Size"].get<u32>(),
                .Offset = uniform["Offset"].get<u32>(),
                .Value = UniformValue{uniform["Value"]}
            };

            uniforms.push_back(info);
        }

        return Material::Create({
            .Name = materialPath,
            .Shader = *Renderer::Shader::Create({
                .Name = "Preview Shader",
                .Binary = shaderBinary,
                .EntryPoint = "main"
            }),
            .Textures = textures,
            .UniformBufferSize = materialInfo["UniformBufferSize"],
            .Uniforms = uniforms
        });
    }
}
