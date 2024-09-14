#include "MaterialThumbnailGenerator.h"

#include "../Editor.h"
#include <Insight/Assets/Material.h>

#include "Insight/Assets/StaticMesh.h"
#include "Insight/Components/CameraComponent.h"
#include "Insight/Components/EnvironmentComponent.h"
#include "Insight/Components/PointLightComponent.h"
#include "Insight/Components/StaticMeshComponent.h"
#include "Insight/Components/TransformComponent.h"
#include "Insight/Geometry/BoxGeometry.h"
#include "Insight/Geometry/SphereGeometry.h"
#include "Insight/Renderer/SceneRenderer.h"


namespace Portent::Thumbnails
{
    Ref<Texture> MaterialThumbnailGenerator::GenerateThumbnail(const Ref<Asset> asset, uvec2 size) const
    {
        if (asset->GetType() != AssetType::Material)
        {
            return nullptr;
        }

        auto material = static_pointer_cast<Material>(asset);

        // create a new image with the desired size

        auto thumbnail = Renderer::Image::Create({
            .Name = "Thumbnail: " + material->GetName(),
            .Extent = {size.x, size.y, 1},
            .Format = vk::Format::eR8G8B8A8Srgb,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eSampled,
            .Filter = vk::Filter::eLinear,
        });

        auto thumbnailView = Renderer::ImageView::Create({
            .Image = *thumbnail,
            .Name = "Thumbnail View: " + material->GetName(),
        });

        const auto commandBuffer = Renderer::CommandBuffer::Create();
        commandBuffer->Begin(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        Renderer::SceneRenderer renderer(vk::Format::eR8G8B8A8Srgb);

        renderer.SetExtent(size);
        renderer.CreateResources();

        Scene scene;
        Entity camera = scene.CreateEntity("Camera");
        camera.AddComponent<CameraComponent>().SetPerspectiveProjection(
            45,
            size.x / size.y,
            0.1f,
            8192);
        camera.GetComponent<TransformComponent>().Position = {12, 0, 0};
        camera.GetComponent<TransformComponent>().LookAt({0, 0, 0});

        Entity environment = scene.CreateEntity("Environment");
        environment.AddComponent<EnvironmentComponent>().Environment->SetEnvironmentCubemap(Editor::Instance().GetResources().GetHDRITextures()[0]);

        SphereGeometry geometry({
            .Radius = 4,
            .WidthSegments = 64,
            .HeightSegments = 64,
        });

        const auto staticMesh = StaticMesh::Create({
            .Name = "Preview",
            .Vertices = geometry.GetVertices(),
            .Indices = geometry.GetIndices(),
            .Submeshes = geometry.GetSubmeshes(),
            .Materials = {
                material
            },
            .KeepData = false
        });

        Entity entity = scene.CreateEntity("Entity");
        entity.AddComponent<StaticMeshComponent>(staticMesh).Mesh = staticMesh;
        entity.GetComponent<TransformComponent>().Position = {0, 0, 0};

        renderer.RenderScene({
            .Camera = camera,
            .Scene = scene,
            .Target = *thumbnail,
            .CommandBuffer = *commandBuffer,
            .TargetView = *thumbnailView,
        });

        commandBuffer->End();

        Renderer::Context::Instance().SubmitImmediateCommands(*commandBuffer);

        return Texture::Create(thumbnail, {
            .Filter = vk::Filter::eNearest,
            .Format = thumbnail->GetFormat(),
            .MipLevels = 1,
            .AddressMode = vk::SamplerAddressMode::eClampToEdge,
            .Name = thumbnail->GetName(),
        });
    }
}
