#include "res/asset/Texture.hpp"

#include "glad/glad.h"

#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_ONLY_PNG
#define  STBI_ONLY_JPEG
#include "stb_image.h"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/res/Texture.hpp"
#include "util/fun/msg/mtrs_message.hpp"

#include <string>

namespace mtrs::res
{

Texture::Texture(RESOURCE_ARGS)
{
    prs::Texture texture;
    std::memcpy(&texture, file_data, sizeof(texture));
    
    std::string path;
    prs::set_mtrs_to_var(file_ddata[texture.path], path);
    path = dir_pack + path;

    _max_instances = texture.max_instances;

    _number = 0;
    // auto resultInt = obj["number"].get_int64();
    // if(!resultInt.error())
    //     _number = resultInt.value();

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char *pixels = stbi_load(path.c_str(), &_width, &_height, &channels, 0);

#ifndef FLAG_RELEASE
    if (!pixels)
    {
        msg::mtrs_error("Failed to load texture: ", path);
        return;
    }
#endif

    switch (channels)
    {
    case 4: _mode = GL_RGBA; break;
    case 3: _mode = GL_RGB;  break;
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

    stbi_image_free(pixels);
}

Texture::Texture(Texture &&other) noexcept
{
    _ID = other._ID;
    other._ID = 0;
    _mode = other._mode;
    _width = other._width;
    _height = other._height;
    _number = other._number;
    _max_instances = other._max_instances;
}

Texture &Texture::operator=(Texture &&other) noexcept
{
    if(this != &other)
    {
        _ID = other._ID;
        other._ID = 0;
        _mode = other._mode;
        _width = other._width;
        _height = other._height;
        _number = other._number;
        _max_instances = other._max_instances;
    }
    return *this;
}

Texture::~Texture()
{
    glDeleteTextures(1, &_ID);
}

uint32_t Texture::get_prs_size_imp() noexcept
{
    return sizeof(prs::Texture);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture::active() const
{
    glActiveTexture(GL_TEXTURE0 + _number);
}

uint32_t Texture::id() const noexcept
{
    return _ID;
}

uint64_t Texture::max_instances() const noexcept
{
    return _max_instances;
}

int Texture::width() const noexcept
{
    return _width;
}

int Texture::height() const noexcept
{
    return _height;
}

}
