#include "Shader.h"

#include "Context.h"

namespace Insight::Renderer
{
    Shader::Shader(const ShaderInfo& info) : m_Name(info.Name), m_EntryPoint(info.EntryPoint)
    {
        const vector<u8> code = Fs::ReadBinaryFile(std::filesystem::absolute(info.Path));

        vk::ShaderModuleCreateInfo createInfo {
            .codeSize = code.size(),
            .pCode = reinterpret_cast<const u32*>(code.data()),
        };

        m_VkModule = Context::Instance().GetVkDevice().createShaderModule(createInfo);
    }

    Shader::Shader(const ShaderBinaryInfo& info) : m_Name(info.Name), m_EntryPoint(info.EntryPoint)
    {
        vk::ShaderModuleCreateInfo createInfo {
            .codeSize = info.Binary.size_bytes(),
            .pCode = reinterpret_cast<const u32*>(info.Binary.data()),
        };

        m_VkModule = Context::Instance().GetVkDevice().createShaderModule(createInfo);
    }

    Shader::~Shader()
    {
        Context::Instance().GetVkDevice().destroyShaderModule(m_VkModule);
    }

}
