#include "../external/jsonUtils.h"

#include <charconv>

#include <fstream>
#include <sstream>

#include <algorithm>

std::string exprDeploy(std::string_view expr, std::unordered_map<std::string, std::string>& defines)
{
    size_t posOperator = expr.find_first_of("*/");
    if(posOperator == std::string::npos)
        posOperator = expr.find_first_of("+-");
    
    if(posOperator == std::string::npos)
    {
        auto it = defines.find(std::string(expr));
        if (it != defines.end()) return it->second;
        return std::string(expr);
    }

    std::string operand_1 = exprDeploy(expr.substr(0, posOperator), defines);
    std::string operand_2 = exprDeploy(expr.substr(posOperator + 1), defines);
    int value_1, value_2;

    auto result = std::from_chars(operand_1.data(), operand_1.data() + operand_1.length(), value_1);
#   ifndef FLAG_RELEASE
    if(result.ec != std::errc()) std::cerr << "Failed convert \"" << operand_1 << "\" to int\n";  
#   endif
    result = std::from_chars(operand_2.data(), operand_2.data() + operand_2.length(), value_2);
#   ifndef FLAG_RELEASE
    if(result.ec != std::errc()) std::cerr << "Failed convert \"" << operand_2 << "\" to int\n";  
#   endif

    switch (expr[posOperator])
    {
    case '*': value_1 *= value_2; break;
    case '/': value_1 = value_2 ? value_1 / value_2 : 0; break;
    case '+': value_1 += value_2; break;
    case '-': value_1 -= value_2; break;
    }

    return std::to_string(value_1);
}

simdjson::padded_string preprocessJSON(const std::string& path,
                           std::unordered_map<std::string, std::string>& defines)
{
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot open " + path);

    std::ostringstream result;
    std::string line;

    while (std::getline(file, line))
    {
        line.erase(0, line.find_first_not_of(" \t"));

        if (line.rfind("\"#define", 0) == 0)
        {
            std::istringstream iss(line);
            std::string define, macros, value;
            iss >> define >> macros >> value;
            defines[macros] = value;
            continue;
        }

        if (line.rfind("#include", 0) == 0)
        {
            std::string includePath;
            size_t start = line.find('"') + 1;
            size_t end = line.find_last_of('"');
            includePath = line.substr(start, end - start);

            result << preprocessJSON(includePath, defines) << "\n";
            continue;
        }

        result << line << "\n";
    }

    std::string text = result.str();

    for (auto& [macros, value] : defines)
    {
        size_t pos = text.find(macros, 0);
        while (pos != std::string::npos)
        {
            pos = text.rfind('"', pos) + 1;
            std::string expr = text.substr(pos, text.find('"', pos + value.length()) - pos);
            size_t exprLength = expr.length();
            expr.erase(std::remove(expr.begin(), expr.end(), ' '), expr.end());
            std::string result = exprDeploy(expr, defines);
            text.replace(pos - 1, exprLength + 2, result);
            pos += result.length() - 1;
            pos = text.find(macros, pos);
        }
    }

    return simdjson::padded_string(text.data(), text.length());
}