#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <string>
#include <vector>
#include <unordered_map>
#include "simdjson.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

simdjson::padded_string preprocessJSON(const std::string& path, std::unordered_map<std::string, std::string>& defines);

template<typename T>
T getVarJSON(simdjson::ondemand::value var)
{
    auto result = var.get<T>();
#ifndef FLAG_RELEASE
    if(result.error())
    {
        std::cerr << "Fatal error find " << typeid(T).name() << std::endl
                << "Error: " << simdjson::error_message(result.error()) << std::endl;
    }
#endif
    return result.value();
}

#endif
