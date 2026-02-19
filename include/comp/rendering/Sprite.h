#ifndef SPRITE_H
#define SPRITE_H

#include "jsonUtils.h"

#include "glm/vec4.hpp"

#include "res/ResourceManager.h"
#include "res/RenderContext.h"
#include "res/ShaderProgram.h"
#include "res/Texture.h"
#include "res/TextureAtlas.h"

typedef unsigned long EntityID;
class ECSWorld;

struct Sprite
{
    std::shared_ptr<ShaderProgram> shader;
    std::shared_ptr<Texture> texture;
    std::shared_ptr<TextureAtlas> atlas;

    TextureAtlas::SubTexture2D subTexture;
    glm::tvec4<uint8_t, glm::precision::highp> color{255, 255, 255, 255};
    glm::vec2 size;
    float layer = 0.f;

    bool visibility =  true;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::string_view name = get_var_json<std::string_view>(obj["shaderName"]);
        std::string_view json = get_var_json<std::string_view>(obj["shaderJSON"]);
        shader = resource.get_resource<ShaderProgram>(std::string(name), std::string(json), "shaders");

        name = get_var_json<std::string_view>(obj["textureName"]);
        json = get_var_json<std::string_view>(obj["textureJSON"]);
        texture = resource.get_resource<Texture>(std::string(name), std::string(json), "textures");

        std::shared_ptr<RenderContext> context = resource.get_cache<RenderContext>()["context"].lock();
        context->create_sprite_batch(shader, texture);

        if(set_var_json(name, obj["atlasName"]) && set_var_json(json, obj["atlasJSON"]))
        {
            atlas = resource.get_resource<TextureAtlas>(std::string(name), std::string(json), "atlases");
            subTexture = atlas->get_sub_texture(0);
        }
        else
        {
            atlas = nullptr;
            subTexture = TextureAtlas::SubTexture2D{glm::vec2(0.001f), glm::vec2(0.999f)};
        }

        layer = get_var_json<double>(obj["layer"]);

        size.x = get_var_json<int64_t>(obj["size_x"]);
        size.y = get_var_json<int64_t>(obj["size_y"]);

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
