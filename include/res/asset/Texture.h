#ifndef TEXTURE_H
#define TEXTURE_H

#include "jsonUtils.h"

class ResourceManager;

class Texture
{
    unsigned int _ID;
    unsigned char _number;
    int _mode;
    int _width;
    int _height;

public:
    void from_json(simdjson::ondemand::object obj, ResourceManager &resource);
    Texture() = default;
    Texture(Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&other) noexcept;
    Texture &operator=(Texture &&other) noexcept;
    ~Texture();

    void bind() const;
    void active() const;
    unsigned int id() const noexcept;
};


#endif
