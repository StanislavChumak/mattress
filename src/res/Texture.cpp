#include "res/Texture.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../external/stb_image.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

void Texture2D::from_json(simdjson::ondemand::object obj, ResourceManager &resource)
{
    stbi_set_flip_vertically_on_load(true);

    auto resultInt = obj["number"].get_int64();
    if(!resultInt.error())
        _number = resultInt.value();
    
    std::string path = std::string(get_var_json<std::string_view>(obj["filePath"]));

    int channels = 0;
    unsigned char *pixels = stbi_load(path.c_str(), &_width, &_height, &channels, 0);

#ifndef FLAG_RELEASE
    if (!pixels) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }
#endif
    
    switch (channels)
    {
    case 4:
        _mode = GL_RGBA;break;
    case 3:
        _mode = GL_RGB;
    }
    glGenTextures(1, &_ID);
    glActiveTexture(GL_TEXTURE0 + _number);
    glBindTexture(GL_TEXTURE_2D, _ID);
    glTexImage2D(GL_TEXTURE_2D, 0, _mode, _width, _height, 0, _mode, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D &&other) noexcept
{
    _ID = other._ID;
    other._ID = 0;
    _mode = other._mode;
    _width = other._width;
    _height = other._height;
    _number = other._number;
}

Texture2D &Texture2D::operator=(Texture2D &&other) noexcept
{
    if(this != &other)
    {
        glDeleteTextures(1, &_ID);
        _ID = other._ID;
        other._ID = 0;
        _mode = other._mode;
        _width = other._width;
        _height = other._height;
        _number = other._number;
    }
    return *this;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &_ID);
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture2D::active() const
{
    glActiveTexture(GL_TEXTURE0 + _number);
}

unsigned int Texture2D::id() const noexcept
{
    return _ID;
}
