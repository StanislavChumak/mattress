#include "res/ResourceManager.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

std::shared_ptr<simdjson::padded_string> ResourceManager::get_json(const std::string& path)
{
    std::shared_ptr<simdjson::padded_string> json;
    if(auto it = _jsonCache.find(path); it != _jsonCache.end())
        return it->second;
    
    std::unordered_map<std::string, std::string> defines;
    return _jsonCache[path] = std::make_shared<simdjson::padded_string>(std::move(preprocess_json(path, defines)));
    
}

void ResourceManager::garbage_collector()
{
    for(auto &collector : _garbageCollectors)
        collector();
}

