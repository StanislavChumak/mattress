#ifndef SPRITE_MAP_H
#define SPRITE_MAP_H

#include "jsonUtils.h"

#include "glm/vec4.hpp"

#include "res/ResourceManager.h"
#include "res/RenderContext.h"
#include "res/ShaderProgram.h"
#include "res/Texture.h"
#include "res/TextureAtlas.h"

#include <vector>

typedef unsigned long EntityID;
class ECSWorld;

struct SpriteMap
{
    std::shared_ptr<ShaderProgram> shader;
    std::shared_ptr<TextureAtlas> atlas;

    struct MapCell
    {
        glm::ivec2 cord;
        TextureAtlas::SubTexture2D subTexture;
    };
    std::vector<MapCell> map;

    glm::vec2 cellSize;
    glm::vec2 offset;
    glm::tvec4<uint8_t, glm::precision::highp> color{255, 255, 255, 255};
    float layer = 0.f;

    bool visibility =  true;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        
    }
};


#endif
