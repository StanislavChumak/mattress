#ifndef GET_FROM_FILE_MTRS_H
#define GET_FROM_FILE_MTRS_H

#include <string>
#include <cstdint>
#include <iosfwd>

std::string get_string_from_mtformat(std::ifstream &file, uint32_t offset, uint32_t size);

#endif
