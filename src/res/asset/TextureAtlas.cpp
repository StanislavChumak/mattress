#include "res/asset/TextureAtlas.h"

#include "res/ResourceManager.h"

#include "mtrsstruct/dynamic_field.def"
#include "mtrsstruct/res_struct/TextureAtlas.struct"

namespace mtrs::res
{

TextureAtlas::TextureAtlas(std::ifstream &file)
{
    TextureAtlas_rs atlas;
    file.read(reinterpret_cast<char*>(&atlas), sizeof(atlas));

    uint32_t count = (atlas.height / atlas.subHeight) * (atlas.width / atlas.subWidth);
    uint64_t currentTextureOffsetX = 0;
    uint64_t currentTextureOffsetY = atlas.height;
    _atlas.clear();
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / atlas.width, static_cast<float>(currentTextureOffsetY - atlas.subHeight + 0.01f) / atlas.height);
        glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + atlas.subWidth - 0.01f) / atlas.width, static_cast<float>(currentTextureOffsetY - 0.01f) / atlas.height);

        _atlas.push_back(SubTexture2D{leftBottom, rigthTop});

        currentTextureOffsetX += atlas.subWidth;
        if (currentTextureOffsetX >= atlas.width)
        {
            currentTextureOffsetX = 0;
            currentTextureOffsetY -= atlas.subHeight;
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

TextureAtlas::~TextureAtlas()
{
    _atlas.clear();
}

std::string TextureAtlas::get_type_name()
{
    return "atlases";
}

uint32_t TextureAtlas::get_type_size()
{
    return sizeof(TextureAtlas_rs);
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

}