#include "res/ResourceManager.h"

#include "util/get_files_from_folder.h"

namespace mtrs::res
{

ResourceManager::ResourceManager(std::string executablePath, std::string pathResDir)
:_executablePath(executablePath)
{
    auto files = get_files_from_folder(pathResDir, ".mtrs");
    _resourcePacks.reserve(files.size());
    _resourcePackItrators.reserve(files.size());
    for(auto &path : files)
    {
        _resourcePacks.emplace(path, ResourcePack{std::ifstream(), 0});
        _resourcePackItrators.push_back(_resourcePacks.find(path));
    }
}

ResourceManager::ResourceManager(ResourceManager &&other) noexcept
{
    _executablePath = std::move(other._executablePath);
    _resourcePacks = std::move(other._resourcePacks);
    _resourcePackItrators = std::move(other._resourcePackItrators);
    _garbageCollectors = std::move(other._garbageCollectors);
}

ResourceManager &ResourceManager::operator=(ResourceManager &&other) noexcept
{
    if(this != &other)
    {
        _executablePath = std::move(other._executablePath);
        _resourcePacks = std::move(other._resourcePacks);
        _resourcePackItrators = std::move(other._resourcePackItrators);
        _garbageCollectors = std::move(other._garbageCollectors);
    }
    return *this;
}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::move_to_resource(std::ifstream &file, const std::string &resName,
        const std::string &resTypeName, uint32_t resTypeSize)
{
    file.seekg(16, std::ios::beg);

        char *name;
        uint32_t size, offset, blockSize;
        std::streampos cursorBuffer;
        while(!file.eof())
        {
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            name = new char[size];
            file.read(name, size);
            file.read(reinterpret_cast<char*>(&blockSize), sizeof(blockSize));
            if(name == resTypeName)
                break;
            file.seekg(blockSize, std::ios::cur);
        }
        delete(name);

        int count = blockSize / resTypeSize;
        for(int i=0; i < count; i++)
        {
            file.read(reinterpret_cast<char*>(&size), sizeof(size));
            if(resName.size() == size)
            {
                name = new char[size];
                file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
                cursorBuffer = file.tellg();
                file.seekg(offset, std::ios::beg);
                file.read(name, size);
                if(name == resName)
                {
                    return;
                }
                delete(name);
            }
            file.seekg(resTypeSize - sizeof(size), std::ios::cur);
        }

#ifdef FLAG_DEBUG
        std::cerr << "!= Error: Failed to get resource(" << resTypeName << ": " << resName
            << ") from resource pack" << std::endl;
        file.close();
        
#endif
}

void ResourceManager::garbage_collector()
{
    for(auto &collector : _garbageCollectors)
    {
        collector();
    }

    for(auto &iter : _resourcePackItrators)
    {
        if(iter->second.resourceCount == 0 && iter->second.file.is_open())
        {
            _resourcePacks[iter->first].file.close();
        }
    }
    
}

};
