#ifndef JSON_MANAGER_H
#define JSON_MANAGER_H

#include <string>
#include <unordered_map>

#include "rapidjson/document.h"

rapidjson::Document getDocumentFromJSON(std::string json);

std::string preprocessJSON(const std::string& path, std::unordered_map<std::string, std::string>& defines);

std::string exprDeploy(std::string_view expression, std::unordered_map<std::string, std::string>& defines);

#endif
