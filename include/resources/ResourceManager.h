#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../external/jsonUtils.h"

#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

class ResourceManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<simdjson::padded_string>> _jsonCache;

    std::vector<std::function<void()>> _garbageCollectors;

    template<typename Resource>
    struct cacheWrapper
    {
        std::unordered_map<std::string, std::weak_ptr<Resource>> cache;
        bool initialization = false;
    };

public:
    template<typename Resource>
    std::unordered_map<std::string, std::weak_ptr<Resource>> &getCache()
    {
        static cacheWrapper<Resource> wrapper;
        if(!wrapper.initialization)
        {
            _garbageCollectors.push_back([&]()
            {
                for (auto it = wrapper.cache.begin(); it != wrapper.cache.end(); )
                    if (it->second.expired())
                        it = wrapper.cache.erase(it);
                    else
                        ++it;
            });
            wrapper.initialization = true;
        }
        return wrapper.cache;
    }

    void garbageCollector();

    std::shared_ptr<simdjson::padded_string> getJSON(const std::string& path);

    template<typename Resource>
    std::shared_ptr<Resource> getResource(const std::string &resourceName,
                                        const std::string& patch,
                                        const std::string& arrayKey)
    {
        auto &cache = getCache<Resource>();
        auto it = cache.find(resourceName);
        if(it != cache.end())
            if(auto existing = it->second.lock())
                return std::static_pointer_cast<Resource>(existing);
        std::shared_ptr<simdjson::padded_string> json = getJSON(patch);
        simdjson::ondemand::parser parser;
        simdjson::ondemand::document doc = parser.iterate(*json);

        for(simdjson::ondemand::object obj : getVarJSON<simdjson::ondemand::array>(doc[arrayKey]))
        {
            std::string_view name = getVarJSON<std::string_view>(obj["name"]);
            if(name == resourceName)
            {
                std::shared_ptr<Resource> resource = std::make_shared<Resource>();
                resource->fromJson(obj, *this);
                cache[resourceName] = resource;
                return resource;
            }
        }

        throw std::runtime_error(std::string("Can't find the ") + resourceName + arrayKey);
    }
};

#endif
