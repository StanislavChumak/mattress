#ifndef SPRITE_MAP_H
#define SPRITE_MAP_H

#include "../external/jsonUtils.h"

#include "glm/vec4.hpp"

#include "resources/ResourceManager.h"
#include "resources/RenderContext.h"
#include "resources/ShaderProgram.h"
#include "resources/Texture2D.h"
#include "resources/TextureAtlas.h"

#include <vector>

typedef unsigned long EntityID;
class ECSWorld;

struct SpriteMap
{
    std::shared_ptr<ShaderProgram> shader;
    std::shared_ptr<Texture2D> texture;
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
        std::string_view name = getVarJSON<std::string_view>(obj["shaderName"]);
        std::string_view json = getVarJSON<std::string_view>(obj["shaderJSON"]);
        shader = resource.getResource<ShaderProgram>(std::string(name), std::string(json), "shaders");

        name = getVarJSON<std::string_view>(obj["textureName"]);
        json = getVarJSON<std::string_view>(obj["textureJSON"]);
        texture = resource.getResource<Texture2D>(std::string(name), std::string(json), "textures");

        std::shared_ptr<RenderContext> context = resource.getCache<RenderContext>()["context"].lock();
        context->createSpriteBatch(shader, texture);

        if(setVarJSON(name, obj["atlasName"]) && setVarJSON(json, obj["atlasJSON"]))
        {
            atlas = resource.getResource<TextureAtlas>(std::string(name), std::string(json), "atlases");
            //subTexture = atlas->getSubTexture(0);
        }
        else
        {
            atlas = nullptr;
            //subTexture = TextureAtlas::SubTexture2D{glm::vec2(0.001f), glm::vec2(0.999f)};
        }

        layer = getVarJSON<double>(obj["layer"]);

        // size.x = getVarJSON<int64_t>(obj["size_x"]);
        // size.y = getVarJSON<int64_t>(obj["size_y"]);

        auto result = obj["color"].get_array();
        if(!result.error())
        {
            std::vector<float> bufferColor;
            bufferColor.reserve(4);
            for(auto rgb : result.value())
                bufferColor.push_back(rgb.get_double().value());
            
            color.r = bufferColor[0];
            color.g = bufferColor[1];
            color.b = bufferColor[2];
            color.a = bufferColor[3];
        }        
    }
};


#endif
