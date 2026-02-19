#include "util/get_files_from_folder.h"

#include <filesystem>

#ifdef FLAG_DEBUG
#include <iostream>
#endif

namespace fs = std::filesystem;

std::unordered_set<std::string> get_files_from_folder(const std::string &dirPath, const std::string &extension)
{
    std::unordered_set<std::string> files;
    try
    {
        for (const auto& entry : fs::directory_iterator(dirPath))
        {
            fs::path p = entry.path();
            if (entry.is_directory())
            {
                files.merge(get_files_from_folder(p, extension));
            }
            else if (entry.is_regular_file() && p.extension() == extension)
            {
                files.emplace(p);
            }
        }
    }
    catch (const fs::filesystem_error& e)
    {
#ifdef FLAG_DEBUG
        std::cerr << "Error: " << e.what() << std::endl;
#endif
    }
    return files;
}