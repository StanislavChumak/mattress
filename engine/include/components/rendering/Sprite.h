#ifndef SPRITE_H
#define SPRITE_H

#include "../external/jsonUtils.h"

#include "resources/ResourceManager.h"
#include "resources/RenderContext.h"
#include "resources/ShaderProgram.h"
#include "resources/Texture2D.h"
#include "resources/VertexArrayObject.h"
#include "resources/BufferObject.h"

#include "glm/vec4.hpp"

typedef unsigned long EntityID;
class ECSWorld;

struct Sprite2D
{
    std::shared_ptr<Texture2D> texture;
    std::shared_ptr<ShaderProgram> shader;

    Texture2D::SubTexture2D subTexture;
    glm::tvec4<uint8_t, glm::precision::highp> color{255, 255, 255, 255};
    glm::vec2 size;
    float layer = 0.f;

    bool visibility =  true;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::string shaderName = std::string(getVarJSON<std::string_view>(obj["shaderName"]));
        std::string shaderJSON = std::string(getVarJSON<std::string_view>(obj["shaderJSON"]));
        std::string textureName = std::string(getVarJSON<std::string_view>(obj["textureName"]));
        std::string textureJSON = std::string(getVarJSON<std::string_view>(obj["textureJSON"]));

        layer = getVarJSON<double>(obj["layer"]);

        size.x = getVarJSON<int64_t>(obj["size_x"]);
        size.y = getVarJSON<int64_t>(obj["size_y"]);

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
        shader = resource.getResource<ShaderProgram>(shaderName, shaderJSON, "shaders");
        texture = resource.getResource<Texture2D>(textureName, textureJSON, "textures");

        subTexture = texture->getSubTexture(0);

        std::shared_ptr<RenderContext> context = resource.getCache<RenderContext>()["context"].lock();
        u_int64_t batcheId = shader->id() | u_int64_t(texture->id()) << 32;
        if (context->batches.find(batcheId) == context->batches.end())
        {
            context->batches.emplace(batcheId, SpriteBatch(context, shader, texture));
        }
    }
};

#endif
