#include "res/asset/TextureAtlas.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/Texture.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/fun/math/hash.hpp"
#include "util/type/prs/res/TextureAtlas.hpp"

#include <string>

namespace mtrs::res
{

std::vector<SubTexture> get_classic_atlas(glm::uvec2 size, glm::uvec2 sub_size)
{
    std::vector<SubTexture> sub_textures;

    glm::uvec2 current_offset = glm::uvec2{0, size.y};
    uint32_t count = (size.y / sub_size.y) * (size.x / sub_size.x);

    sub_textures.clear();
    sub_textures.reserve(count);
    for(size_t i = 0; i < count; i++)
    {
        glm::vec2 left_bottom(static_cast<float>(current_offset.x + 0.01f) / size.x,
            static_cast<float>(current_offset.y - sub_size.y + 0.01f) / size.y);
        glm::vec2 rigth_top(static_cast<float>(current_offset.x + sub_size.x - 0.01f) / size.x, 
            static_cast<float>(current_offset.y - 0.01f) / size.y);

        sub_textures.push_back(SubTexture{left_bottom, rigth_top});

        current_offset.x += sub_size.x;
        if (current_offset.x >= size.x)
        {
            current_offset.x = 0;
            current_offset.y -= sub_size.y;
        }
    }

    return sub_textures;
}

std::vector<SubTexture> get_spiral_atlas(glm::uvec2 size, glm::uvec2 sub_size)
{
    std::vector<SubTexture> sub_textures;

    glm::uvec2 current_offset = glm::uvec2{0, size.y};

    enum class Direction
    {
        Right,
        Left,
        Top,
        Botton
    };

    Direction direction = Direction::Right;

    sub_textures.clear();
    while (sub_size.x > 0 && sub_size.y > 0)
    {
        glm::vec2 left_bottom(static_cast<float>(current_offset.x + 0.01f) / size.x,
            static_cast<float>(current_offset.y - sub_size.y + 0.01f) / size.y);
        glm::vec2 rigth_top(static_cast<float>(current_offset.x + sub_size.x - 0.01f) / size.x, 
            static_cast<float>(current_offset.y - 0.01f) / size.y);

        sub_textures.push_back(SubTexture{left_bottom, rigth_top});

        switch (direction)
        {
        case Direction::Right:
            current_offset.x += sub_size.x;
            direction = Direction::Botton;
            break;
        case Direction::Botton:
            current_offset.x += sub_size.x / 2;
            current_offset.y -= sub_size.y;
            direction = Direction::Left;
            break;
        case Direction::Left:
            current_offset.x -= sub_size.x / 2;
            current_offset.y -= sub_size.y / 2;
            direction = Direction::Top;
            break;
        case Direction::Top:
            current_offset.y += sub_size.y / 2;
            direction = Direction::Right;
            break;
        }

        sub_size.y /= 2;
        sub_size.x /= 2;
    }
    
    return sub_textures;
}

TextureAtlas::TextureAtlas(RESOURCE_ARGS)
{
    prs::TextureAtlas atlas;
    std::memcpy(&atlas, file_data, sizeof(atlas));

    std::string texture_name;
    prs::set_mtrs_to_var(file_ddata[atlas.texture], texture_name);
    std::string pack_name = dir_pack.substr(resources.get_pack_dir().size()) + pack;
    auto texture = resources.get_resource<Texture>(math::hash64(pack_name), texture_name);

    if(atlas.spirality)
    {
        _atlas = get_spiral_atlas({texture->width(), texture->height()},
            {atlas.sub_width, atlas.sub_height});
    }
    else
    {
        _atlas = get_classic_atlas({texture->width(), texture->height()},
            {atlas.sub_width, atlas.sub_height});
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

uint32_t TextureAtlas::get_prs_size_imp() noexcept
{
    return sizeof(prs::TextureAtlas);
}

SubTexture TextureAtlas::get_sub_texture(const size_t index) const
{
#ifndef FLAG_RELEASE
    if (index >= _atlas.size())
    {
        msg::mtrs_error("Fatal find subTexture \"", index , "\"");
        return _atlas[0];
    }
#endif
    return _atlas[index];
}

}