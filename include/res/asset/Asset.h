#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <cstdint>
#include <iosfwd>

#define ASSETS_CONSTRUCTORS(Asset) \
Asset(std::ifstream &file); \
Asset() = delete; \
Asset(Asset &) = delete; \
Asset &operator=(const Asset &) = delete; \
Asset(Asset &&other) noexcept; \
Asset &operator=(Asset &&other) noexcept; \
~Asset()

namespace mtrs::res
{

class Asset
{
public:
    virtual std::string get_type_name() = 0;
    virtual uint32_t get_type_size() = 0;
};

};

#endif
