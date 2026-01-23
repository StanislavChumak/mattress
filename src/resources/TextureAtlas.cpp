#include "resources/TextureAtlas.h"

#include "resources/ResourceManager.h"
#include "resources/Texture2D.h"

void TextureAtlas::fromJson(simdjson::ondemand::object obj, ResourceManager &resource)
{
    std::string textureName = std::string(getVarJSON<std::string_view>(obj["textureName"]));
    std::string textureJSON = std::string(getVarJSON<std::string_view>(obj["textureJSON"]));
    auto texture = resource.getResource<Texture2D>(textureName, textureJSON, "textures");

    size_t count = getVarJSON<int64_t>(obj["subCount"]);
    unsigned int subTextureWidth = getVarJSON<int64_t>(obj["subWidth"]);
    unsigned int subTextureHeight = getVarJSON<int64_t>(obj["subHeight"]);

    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = texture->_height;
    _atlas.clear();
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / texture->_width, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / texture->_height);
        glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / texture->_width, static_cast<float>(currentTextureOffsetY - 0.01f) / texture->_height);

        _atlas.push_back(SubTexture2D{leftBottom, rigthTop});

        currentTextureOffsetX += subTextureWidth;
        if (currentTextureOffsetX >= texture->_width)
        {
            currentTextureOffsetX = 0;
            currentTextureOffsetY -= subTextureHeight;
        }
    }
}

TextureAtlas::TextureAtlas(TextureAtlas &&other) noexcept
{
    _atlas = other._atlas;
    other._atlas.clear();
}

TextureAtlas &TextureAtlas::operator=(TextureAtlas &&other) noexcept
{
    if(this != &other)
    {
        _atlas = other._atlas;
        other._atlas.clear();
    }
    return *this;
}

const TextureAtlas::SubTexture2D &TextureAtlas::getSubTexture(const size_t index) const
{
    if (index >= _atlas.size())
    {
        std::cerr << "Fatal find subTexture \"" << index << "\"\n";
        return _atlas[0];
    }
    return _atlas[index];
}