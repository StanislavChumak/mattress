#include "resources/ResourceManager.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

simdjson::simdjson_result<simdjson::ondemand::document>
ResourceManager::getJSON(const std::string& path, bool isResource)
{
    simdjson::ondemand::parser *parser = &_parser;
    if(isResource) parser = &_resourceParser;
    if(auto it = _jsonCache.find(path); it != _jsonCache.end())
        return parser->iterate(*it->second);

    std::unordered_map<std::string, std::string> defines;
    _jsonCache[path] = std::make_shared<simdjson::padded_string>(std::move(preprocessJSON(path, defines)));

    return parser->iterate(*_jsonCache[path]);
}

void ResourceManager::garbageCollector()
{
    for(auto &collector : _garbageCollectors)
        collector();
}

