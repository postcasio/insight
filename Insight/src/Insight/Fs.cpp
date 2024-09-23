#include "Fs.h"

#include <fstream>
#include <fmt/format.h>
#include <glob/glob.h>

namespace Insight
{
    vector<u8> Fs::ReadBinaryFile(const path& path)
    {

        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error(fmt::format("Can't open file: {}", path.string()));
        }

        const u32 fileSize = (u32)file.tellg();
        std::vector<u8> buffer(fileSize);

        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

        file.close();

        return buffer;
    }

    string Fs::ReadTextFile(const path& path)
    {
        std::ifstream file(path);

        if (!file.is_open())
        {
            throw std::runtime_error(fmt::format("Can't open file: {}", path.string()));
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        file.close();

        return content;
    }

    json Fs::ReadJsonFile(const path& path)
    {
        string content = ReadTextFile(path);

        return json::parse(content);
    }

    void Fs::WriteJsonFile(const path& path, json::const_reference json)
    {
        std::ofstream file(path);
        file << json.dump(4);
        file.close();
    }

    vector<Fs::Entry> Fs::GetDirectoryContents(const path& path)
    {
        vector<Entry> entries;

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            Entry e;

            e.Name = entry.path().filename().string();
            e.Path =  entry.path().string();
            e.Extension = entry.path().extension().string();
            e.IsDirectory = entry.is_directory();

            entries.push_back(e);
        }

        return entries;
    }

    void Fs::CreateDirectory(const path& path)
    {
        std::filesystem::create_directory(path);
    }

    bool Fs::FileExists(const path& path)
    {
        return std::filesystem::exists(path);
    }

    bool Fs::IsDirectory(const path& path)
    {
        return std::filesystem::is_directory(path);
    }

    vector<Fs::Entry> Fs::Glob(const string& pattern)
    {
        vector<Entry> entries;

        for (auto& p : glob::glob(pattern)) {                // e.g., .bash_history, .bashrc
            Entry e;

            e.Name = p.filename().string();
            e.Path = p.string();
            e.Extension = p.extension().string();
            e.IsDirectory = Fs::IsDirectory(p);

            entries.push_back(e);
        }

        return entries;
    }
}
