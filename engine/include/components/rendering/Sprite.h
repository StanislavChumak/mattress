#ifndef SPRITE_H
#define SPRITE_H

#include "rapidjson/document.h"

#include "../../../src/resources/ResourceManager.h"
#include "../../../src/resources/renderer/VertexArrayObject.h"
#include "../../../src/resources/renderer/BufferObject.h"

#include "glm/vec3.hpp"

struct Sprite2D {
    const Texture2D *texture;
    const ShaderProgram *shader;
    Texture2D::SubTexture2D *subTextures;

    float layer = 0.f;

    BufferObject VBO;
    BufferObject VBOtex;
    BufferObject EBO;
    VertexArrayObject VAO;

    bool visibility =  true;

    Sprite2D() = default;

    void fromJson(const rapidjson::Value& j, const ResourceManager &resource)
    {
        shader = resource.getShaderProgram(j["shader"].GetString());
        texture = resource.getTexture2D(j["texture"].GetString());
        auto jsubTextures = j["subTextures"].GetArray();
        subTextures = new Texture2D::SubTexture2D[jsubTextures.Size()];
        for(int i = 0; i < jsubTextures.Size(); i++)
        {
            subTextures[i] = texture->get_sub_texture(jsubTextures[i].GetString());
        }
        std::string nameUsage = j["usage"].GetString();
        glm::vec3 color = glm::vec3(1.f);
        layer = j["layer"].GetFloat();
        auto colorIterator = j.FindMember("color");
        if (colorIterator != j.MemberEnd())
        {
            const auto colorArray = colorIterator->value.GetArray();
            color = glm::vec3(colorArray[0].GetFloat(), colorArray[1].GetFloat(), colorArray[2].GetFloat());
        }
        auto subTexture = *subTextures;



        const GLfloat vertexData[] = {
            //vertex cord   //color
            1.0f, 1.0f,     color.x, color.y, color.z, // right top
            1.0f, 0.0f,     color.x, color.y, color.z, // right bottom
            0.0f, 1.0f,     color.x, color.y, color.z, // left top
            0.0f, 0.0f,     color.x, color.y, color.z  // left bottom
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
