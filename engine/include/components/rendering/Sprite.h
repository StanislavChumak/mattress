#ifndef SPRITE_H
#define SPRITE_H

#include "../external/jsonUtils.h"

#include "resources/ResourceManager.h"
#include "resources/ShaderProgram.h"
#include "resources/Texture2D.h"
#include "resources/VertexArrayObject.h"
#include "resources/BufferObject.h"

#include "glm/vec3.hpp"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

typedef unsigned long EntityID;

struct Sprite2D
{
    std::shared_ptr<Texture2D> texture;
    std::shared_ptr<ShaderProgram> shader;
    std::vector<Texture2D::SubTexture2D> subTextures;

    float layer = 0.f;

    BufferObject VBO;
    BufferObject VBOtex;
    BufferObject EBO;
    VertexArrayObject VAO;

    bool visibility =  true;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::string shaderName = std::string(getVarJSON<std::string_view>(obj["shaderName"]));
        std::string shaderJSON = std::string(getVarJSON<std::string_view>(obj["shaderJSON"]));
        std::string textureName = std::string(getVarJSON<std::string_view>(obj["textureName"]));
        std::string textureJSON = std::string(getVarJSON<std::string_view>(obj["textureJSON"]));

        std::vector<std::string> bufferTextures;
        for(auto subTexture : getVarJSON<simdjson::ondemand::array>(obj["subTextures"]))
        {
            bufferTextures.push_back(std::string(getVarJSON<std::string_view>(subTexture.value())));
        }

        layer = getVarJSON<double>(obj["layer"]);

        std::string nameUsage = std::string(getVarJSON<std::string_view>(obj["usage"]));

        float *color = new float[3]{1, 1, 1};
        auto result = obj["color"].get_array();
        if(!result.error())
        {
            std::vector<float> bufferColor;
            for(auto rgb : result.value())
            {
                bufferColor.push_back(rgb.get_double().value());
            }
            color = bufferColor.data();
        }
        shader = resource.getResource<ShaderProgram>(shaderName, shaderJSON, "shaders");
        texture = resource.getResource<Texture2D>(textureName, textureJSON, "textures");
        subTextures.reserve(bufferTextures.size());
        for(auto nameSubTexture : bufferTextures)
        {
            subTextures.push_back(texture->getSubTexture(nameSubTexture));
        }

        auto subTexture = subTextures[0];
        
        const GLfloat vertexData[] = {
            //vertex cord   //color
            1.0f, 1.0f,     color[0], color[1], color[2], // right top
            1.0f, 0.0f,     color[0], color[1], color[2], // right bottom
            0.0f, 1.0f,     color[0], color[1], color[2], // left top
            0.0f, 0.0f,     color[0], color[1], color[2]  // left bottom
        };
        const GLfloat textureData[] = {
            //texture
            subTexture.rightTopVertex.x, subTexture.rightTopVertex.y,
            subTexture.rightTopVertex.x, subTexture.leftBottomVertex.y,
            subTexture.leftBottomVertex.x, subTexture.rightTopVertex.y,
            subTexture.leftBottomVertex.x, subTexture.leftBottomVertex.y
        };
        const GLuint indices[] = {
            2, 3, 0, // first Triangle
            0, 1, 3  // second Triangle
        };

        GLenum usage;
        if(nameUsage == "stream") usage = GL_STREAM_DRAW;
        else if(nameUsage == "dynamic") usage = GL_DYNAMIC_DRAW;
        else if(nameUsage == "static") usage = GL_STATIC_DRAW;

        VAO.bind();

        VBO.init(GL_ARRAY_BUFFER, vertexData, sizeof(vertexData), usage);
        VBOtex.init(GL_ARRAY_BUFFER, textureData, sizeof(textureData), usage);
        
        VAO.add_buffer_float(VBO, 2, 5, 0);

        VAO.add_buffer_float(VBOtex, 2, 2, 0);

        VAO.add_buffer_float(VBO, 3, 5, 2);

        EBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), usage);
    }
};

#endif
