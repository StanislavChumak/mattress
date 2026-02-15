#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "jsonUtils.h"

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
    std::unordered_map<std::string, std::weak_ptr<Resource>> &get_cache()
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

    void garbage_collector();

    std::shared_ptr<simdjson::padded_string> get_json(const std::string& path);

    template<typename Resource>
    std::shared_ptr<Resource> get_resource(const std::string &resourceName,
                                        const std::string& patch,
                                        const std::string& arrayKey)
    {
        auto &cache = get_cache<Resource>();
        auto it = cache.find(resourceName);
        if(it != cache.end())
            if(auto existing = it->second.lock())
                return std::static_pointer_cast<Resource>(existing);
        std::shared_ptr<simdjson::padded_string> json = get_json(patch);
        simdjson::ondemand::parser parser;
        simdjson::ondemand::document doc = parser.iterate(*json);

        for(simdjson::ondemand::object obj : get_var_json<simdjson::ondemand::array>(doc[arrayKey]))
        {
            std::string_view name = get_var_json<std::string_view>(obj["name"]);
            std::shared_ptr<Resource> resource = std::make_shared<Resource>();
            resource->from_json(obj, *this);
            cache[resourceName] = resource;
            if(name == resourceName)
                return resource;
        }

        throw std::runtime_error(std::string("Can't find the ") + resourceName + arrayKey);
    }
};

#endif
