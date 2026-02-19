#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <fstream>

#ifdef FLAG_DEBUG
#include <iostream>
#endif

namespace mtrs::res
{

class ResourceManager
{
private:
    struct ResourcePack
    {
        std::ifstream file;
        size_t resourceCount;
    };
    std::string _executablePath;
    std::unordered_map<std::string, ResourcePack> _resourcePacks;
    std::vector<decltype(_resourcePacks)::const_iterator> _resourcePackItrators;

    std::vector<std::function<void()>> _garbageCollectors;

    void move_to_resource(std::ifstream &file, const std::string &resName,
        const std::string &resTypeName, uint32_t resTypeSize);
    

    template<typename Resource>
    struct cacheWrapper
    {
        std::unordered_map<std::string, std::weak_ptr<Resource>> cache;
        bool initialization = false;
    };

    template<typename Resource>
    std::unordered_map<std::string, std::weak_ptr<Resource>> &get_cache()
    {
        static cacheWrapper<Resource> wrapper;
        if(!wrapper.initialization)
        {
            _garbageCollectors.push_back([&]()
            {
                for (auto itRes = wrapper.cache.begin(); itRes != wrapper.cache.end(); )
                    if (itRes->second.expired())
                    {
                        itRes = wrapper.cache.erase(itRes);

                        size_t pos = itRes->first.find_last_of("\\/");
                        _resourcePacks[itRes->first.substr(0, pos)].resourceCount--;
                    }
                    else
                        ++itRes;
            });
            wrapper.initialization = true;
        }
        return wrapper.cache;
    }

public:
    ResourceManager(std::string executablePath, std::string pathResDir);
    ResourceManager(ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&other) noexcept;
    ResourceManager &operator=(ResourceManager &&other) noexcept;
    ~ResourceManager();

    void garbage_collector();

    template<typename Resource>
    std::shared_ptr<Resource> get_resource(const std::string &resourcePath)
    {
        auto &cache = get_cache<Resource>();
        auto itRes = cache.find(resourcePath);
        if(itRes != cache.end())
            if(auto existing = itRes->second.lock())
                return std::static_pointer_cast<Resource>(existing);

        size_t pos = resourcePath.find_last_of("\\/");
        auto itPack = _resourcePacks.find(resourcePath.substr(0, pos));
        if(itPack != _resourcePacks.end())
        {
            if(!itPack->second.file.is_open())
                itPack->second.file.open(itPack->first, std::ios::binary);
            
            std::string resTypeName = Resource::get_type_name();
            uint32_t resTypeSize = Resource::get_type_size();

            move_to_resource(itPack->second.file, resourcePath.substr(pos + 1), resTypeName, resTypeSize);
            
            std::shared_ptr<Resource> resource = std::make_shared<Resource>(itPack->second.file);
#ifdef FLAG_DEBUG
            if(!resource)
                std::cerr << "!=== Error: in ResourcePack(" << itPack->first << ')' << std::endl;
            else
#endif
                itPack->second.resourceCount++;
            return resource;
        }
    }
};

}

#endif
