#ifndef TEXTURE_H
#define TEXTURE_H

#include "res/asset/Asset.h"

namespace mtrs::res
{

class ResourceManager;

class Texture : public Asset
{
    unsigned int _ID;
    unsigned char _number;
    int _mode;
    int _width;
    int _height;

public:
    ASSETS_CONSTRUCTORS(Texture);

    std::string get_type_name() override;
    uint32_t get_type_size() override;

    void bind() const;
    void active() const;
    unsigned int id() const noexcept;
};

}

#endif
