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

template<typename T1, typename T2>
bool setVarJSON(T1 &dest, T2 var)
{
    auto result = var.template get<T1>();
    if(!result.error()) var = result.value();
    return result.error();
}

template<typename T1, typename T2>
T1 getResultJSON(T2 result)
{
#ifndef FLAG_RELEASE
    if(result.error())
    {
        std::cerr << "Fatal error find " << typeid(T1).name() << std::endl
                << "Error: " << simdjson::error_message(result.error()) << std::endl;
    }
#endif
    return result.value();
}

template<typename T1, typename T2>
T1 getVarJSON(T2 var)
{
    auto result = var.template get<T1>();
    return getResultJSON<T1>(result);
}

#endif
