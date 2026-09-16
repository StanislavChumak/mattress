#include "comp/rendering/Sprite.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"
#include "res/asset/TextureAtlas.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/Sprite.hpp"
#include "util/fun/math/hash.hpp"

#include <cstring>

#ifndef FLAG_RELEASE
    #define SET_RESOURCE(res, Type, manager, scn_hash, tmp_hash, path) \
        if(path == "") { mtrs::msg::mtrs_error("In scene \"",mtrs::math::rehash64(scn_hash), \
            "\" resource type \"",#Type,"\" has an empty path"); } \
        if(!(res = manager.get_resource<Type>(tmp_hash, path))) \
        { mtrs::msg::mtrs_error("In scene \"",mtrs::math::rehash64(scn_hash), \
            "\" resource type \"",#Type,"\" received nothing along the way"); }
#else
    #define SET_RESOURCE(res, Type, manager, scn_hash, tmp_hash, path) \
        res = manager.get_resource<Type>(tmp_hash, path);
#endif

namespace mtrs::comp
{

Sprite::Sprite(COMPONENT_ARGS)
{
    prs::Sprite sprite;
    std::memcpy(&sprite, file_data, sizeof(sprite));

    std::string path_buffer;

    prs::set_mtrs_to_var(file_ddata[sprite.shader], path_buffer);
    SET_RESOURCE(shader, res::ShaderProgram, resource, scn_hash, tmp_hash, path_buffer)

    prs::set_mtrs_to_var(file_ddata[sprite.texture], path_buffer);
    SET_RESOURCE(texture, res::Texture, resource, scn_hash, tmp_hash, path_buffer)

    prs::set_mtrs_to_var(file_ddata[sprite.atlas], path_buffer);
    if(path_buffer != "")
    {
        SET_RESOURCE(atlas, res::TextureAtlas, resource, scn_hash, tmp_hash, path_buffer)
        sub_texture = atlas->get_sub_texture(0);
    }
    else
    {
        atlas = nullptr; 
        sub_texture = res::SubTexture();
    }

    layer = sprite.layer;

    size.x = sprite.size_x;
    size.y = sprite.size_y;

    std::memcpy(&color, &sprite.color, 4);

    visibility = sprite.visibility;
}

uint32_t Sprite::get_prs_size_imp() noexcept
{
    return sizeof(prs::Sprite);
}

}