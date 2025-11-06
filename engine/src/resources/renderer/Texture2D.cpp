#include "resources/renderer/Texture2D.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../external/stb_image.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

void Texture2D::fromJson(simdjson::ondemand::object obj)
{
    stbi_set_flip_vertically_on_load(true);

    auto resultInt = obj["number"].get_int64();
    if(!resultInt.error())
        _number = resultInt.value();
    
    std::string path = std::string(getVarJSON<std::string_view>(obj["filePath"]));

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

    std::vector<std::string> buffer;
    for(auto subTexture : getVarJSON<simdjson::ondemand::array>(obj["subTextures"]))
    {
        buffer.push_back(std::string(getVarJSON<std::string_view>(subTexture.value())));
    }
    unsigned int subTextureWidth = getVarJSON<int64_t>(obj["subTextureWidth"]);
    unsigned int subTextureHeight = getVarJSON<int64_t>(obj["subTextureHeight"]);
    loadAtlas(buffer, subTextureWidth, subTextureHeight);
}

Texture2D &Texture2D::operator=(Texture2D &&texture2D)
{
    glDeleteTextures(1, &_ID);
    _ID = texture2D._ID;
    texture2D._ID = 0;
    _mode = texture2D._mode;
    _width = texture2D._width;
    _height = texture2D._height;
    _number = texture2D._number;
    return *this;
}

Texture2D::Texture2D(Texture2D &&texture2D)
{
    _ID = texture2D._ID;
    texture2D._ID = 0;
    _mode = texture2D._mode;
    _width = texture2D._width;
    _height = texture2D._height;
    _number = texture2D._number;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &_ID);
}

void Texture2D::loadAtlas(const std::vector<std::string> &subTextureNames, const unsigned int &subTextureWidth, const unsigned int &subTextureHeight)
{
    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = _height;
    for (const auto &subTextureName : subTextureNames)
    {
        glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / _width, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / _height);
        glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / _width, static_cast<float>(currentTextureOffsetY - 0.01f) / _height);

        _atlas.emplace(subTextureName, SubTexture2D{leftBottom, rigthTop});

        currentTextureOffsetX += subTextureWidth;
        if (currentTextureOffsetX >= _width)
        {
            currentTextureOffsetX = 0;
            currentTextureOffsetY -= subTextureHeight;
        }
    }
}

const Texture2D::SubTexture2D &Texture2D::getSubTexture(const std::string &subTextureName) const
{
    auto iterator = _atlas.find(subTextureName);
    if (iterator == _atlas.end())
    {
        std::cerr << "Fatal find subTexture \"" << subTextureName << "\"\n";
    }
    return iterator->second;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}

void Texture2D::active() const
{
    glActiveTexture(GL_TEXTURE0 + _number);
}
