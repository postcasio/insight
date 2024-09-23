#include "TextureSaver.h"

#include "../Editor.h"
#include "Insight/Fs.h"
#include "Insight/Assets/Texture.h"
#include "Insight/Loaders/TextureLoader.h"

#include "../Editors/TextureEditor/TextureEditor.h"

namespace Portent::Savers {
    void TextureSaver::Save(Editors::EditorPanel* editor, const AssetSaveOptions& options)
    {
        const auto textureEditor = dynamic_cast<Editors::TextureEditor*>(editor);

        const auto& library = Editor::Instance().GetProject().GetLibrary();

        json textureData;

        textureData["Id"] = options.Descriptor.Id.ToString();
        textureData["Path"] = textureEditor->GetTexturePath().lexically_proximate(library.GetDirectory() / options.Descriptor.Path);
        textureData["Filter"] = Loaders::GetFilterName(textureEditor->GetFilter());
        textureData["Format"] = Loaders::GetFormatName(textureEditor->GetFormat());
        textureData["AddressMode"] = Loaders::GetSamplerAddressModeName(textureEditor->GetAddressMode());
        textureData["MipLevels"] = textureEditor->GetMipLevels();
        textureData["Format"] = textureEditor->GetFormat();

        Fs::WriteJsonFile(library.GetDirectory() / options.Descriptor.Path, textureData);

        INS_INFO("Wrote texture data to {0}", options.Descriptor.Path.c_str());
    }
}
