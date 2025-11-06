#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "../external/jsonUtils.h"

#include <unordered_map>
#include <memory>

class ResourceManager
{
private:
    std::unordered_map<std::string, std::shared_ptr<simdjson::padded_string>> _jsonCache;
    simdjson::ondemand::parser _resourceParser;
    simdjson::ondemand::parser _parser;

    std::unordered_map<std::string, std::weak_ptr<void>> _resourceCache;
public:
    ResourceManager() = default;

    simdjson::simdjson_result<simdjson::ondemand::document> getJSON(const std::string& path, bool isResource = false);

    void collectUnused();

    template<typename Resource>
    std::shared_ptr<Resource> getResource(const std::string &resourceName,
                                        const std::string& json,
                                        const std::string& arrayKey)
    {
        auto it = _resourceCache.find(resourceName);
        if(it != _resourceCache.end())
            if(auto existing = it->second.lock())
                return std::static_pointer_cast<Resource>(existing);
        
        auto resultDocument = getJSON(json, true);
        if(resultDocument.error())
            throw std::runtime_error("Failed to parse JSON");
        simdjson::ondemand::document doc = std::move(resultDocument.value());

        for(simdjson::ondemand::object obj : getVarJSON<simdjson::ondemand::array>(doc[arrayKey]))
        {
            std::string_view name = getVarJSON<std::string_view>(obj["name"]);
            if(name == resourceName)
            {
                std::shared_ptr<Resource> resource = std::make_shared<Resource>();
                resource->fromJson(obj);
                _resourceCache[resourceName] = resource;
                return resource;
            }
        }

        throw std::runtime_error(std::string("Can't find the ") + resourceName + arrayKey);
    }
};

#endif
