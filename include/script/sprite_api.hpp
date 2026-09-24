#ifndef SPRITE_API_HPP
#define SPRITE_API_HPP

#include "engine_api.hpp"
#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "res/asset/TextureAtlas.hpp"

extern mtrs::EngineAPI *api;

namespace mtrs::res
{

SubTexture TextureAtlas::get_sub_texture(const size_t index) const
{
    return api->atlas_get_sub_texture(this, index);
}

}

#endif
