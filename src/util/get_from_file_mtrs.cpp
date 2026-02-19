#include "util/get_from_file_mtrs.h"

#include <fstream>

std::string get_string_from_mtformat(std::ifstream &file, uint32_t offset, uint32_t size)
{
    std::string str;
    str.reserve(size);
    std::streampos posBuffer = file.tellg();

    file.seekg(offset, std::ios::beg);

    file.read(str.data(), size);

    file.seekg(posBuffer, std::ios::beg);

    return str;
}