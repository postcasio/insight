#pragma once

#include "Portent.h"
#include "Controls/FileBrowser.h"
#include "Insight/Assets/Texture.h"

namespace Portent
{
    class Widgets
    {
    public:
        static bool AssetSelector(const string& label, const Ref<Asset>& asset, const AssetType assetType, const ImVec2& size, float brickWidth, Controls::FileBrowser&
                                  fileBrowser, AssetDescriptor* outSelectedAsset);
        static bool PropertyTreeAssetSelector(const string& label, const Ref<Asset>& asset, const AssetType assetType, Controls::FileBrowser& fileBrowser, AssetDescriptor* outSelectedAsset);
        static bool AssetDragDropTarget(const AssetType type, AssetDescriptor* outSelectedAsset);
        static void AssetDragDropSource(const AssetDescriptor& descriptor);

        static void AssetIconCenteredScreenPos(AssetType asset, const ImVec2& position, const ImVec2& size);
        static void AssetIcon(AssetType type, const ImVec2& size);
        static void AssetThumbnail(const AssetDescriptor& descriptor, const ImVec2& size);
        static void AssetPreview(const AssetDescriptor& descriptor, const ImVec2& size);
        static void AssetBrick(const Ref<Asset>& asset, const AssetType assetType, const ImVec2& size, float brickWidth = -FLT_MIN);
    };
}
