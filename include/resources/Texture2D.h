#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "../external/jsonUtils.h"

class ResourceManager;

class Texture2D
{
    friend class TextureAtlas;
private:
    unsigned int _ID;
    unsigned char _number;
    int _mode;
    int _width;
    int _height;

public:
    void fromJson(simdjson::ondemand::object obj, ResourceManager &resource);
    Texture2D() = default;
    Texture2D(Texture2D &) = delete;
    Texture2D &operator=(const Texture2D &) = delete;
    Texture2D(Texture2D &&other) noexcept;
    Texture2D &operator=(Texture2D &&other) noexcept;
    ~Texture2D();

    void bind() const;
    void active() const;
    unsigned int id() const noexcept;
};


#endif
