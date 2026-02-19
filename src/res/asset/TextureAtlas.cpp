#include "res/TextureAtlas.h"

#include "res/ResourceManager.h"

void TextureAtlas::from_json(simdjson::ondemand::object obj, ResourceManager &resource)
{
    int64_t textureWidth = get_var_json<int64_t>(obj["textureWidth"]);
    int64_t textureHeight = get_var_json<int64_t>(obj["textureHeight"]);

    size_t count = get_var_json<int64_t>(obj["subCount"]);
    int64_t subTextureWidth = get_var_json<int64_t>(obj["subWidth"]);
    int64_t subTextureHeight = get_var_json<int64_t>(obj["subHeight"]);

    int64_t currentTextureOffsetX = 0;
    int64_t currentTextureOffsetY = textureHeight;
    _atlas.clear();
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
        glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

        _atlas.push_back(SubTexture2D{leftBottom, rigthTop});

        currentTextureOffsetX += subTextureWidth;
        if (currentTextureOffsetX >= textureWidth)
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

const TextureAtlas::SubTexture2D &TextureAtlas::get_sub_texture(const size_t index) const
{
    if (index >= _atlas.size())
    {
        std::cerr << "Fatal find subTexture \"" << index << "\"\n";
        return _atlas[0];
    }
    return _atlas[index];
}