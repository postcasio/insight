#pragma once

#include <filesystem>

#include "AssetDescriptor.h"
#include "Insight.h"

namespace Insight
{
    using std::filesystem::path;

    class Fs
    {

    public:
        struct Entry
        {
            string Name;
            string Path;
            string Extension;
            bool IsDirectory;
        };

        static vector<u8> ReadBinaryFile(const path& path);

        static string ReadTextFile(const path& path);

        static json ReadJsonFile(const path& path);
        static void WriteJsonFile(const path& path, json::const_reference json);

        static vector<Entry> GetDirectoryContents(const path& path);
        static void CreateDirectory(const path& path);
        static bool FileExists(const path& path);
        static bool IsDirectory(const path& path);
        static vector<Entry> Glob(const string& pattern);

    };
}
