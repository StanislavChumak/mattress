#include "ResourceManager.h"

#include "glad/glad.h"

#include "glm/vec3.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../../external/stb_image.h"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"

#include <fstream>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
    clear();
}

void ResourceManager::clear()
{
    _shaderProgramsMap.clear();
    _textures2DMap.clear();
    _soundsMap.clear();
}

void ResourceManager::setExecutablePath(const std::string &executablePath)
{
    size_t found = executablePath.find_last_of("/\\");
    _executablePath = executablePath.substr(0, found + 1);
}

std::string ResourceManager::getFileString(const std::string &relativeFilePath)
{
    std::ifstream fileFlow;
    fileFlow.open(_executablePath + relativeFilePath, std::ios::ate | std::ios::binary);

#   ifndef FLAG_RELEASE
    if (!fileFlow.is_open())
    {
        std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
        return "";
    }
#   endif

    std::streamsize size = fileFlow.tellg();
    fileFlow.seekg(0);

    char* buffer = new char[size + 1];
    fileFlow.read(buffer, size);
    buffer[size] = '\0';

    return std::string(buffer);
}

ShaderProgram *ResourceManager::loadShaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexShaderCode = getFileString(vertexPath);
    std::string fragmentShaderCode = getFileString(fragmentPath);

#   ifndef FLAG_RELEASE
    if (vertexShaderCode.empty())
    {
        std::cerr << "No vertex shader !" << std::endl;
        return nullptr;
    }
    
    if (fragmentShaderCode.empty())
    {
        std::cerr << "No fragment shader !" << std::endl;
        return nullptr;
    }
#   endif

    ShaderProgram *newShader = _shaderProgramsMap.emplace(shaderName, std::make_unique<ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second.get();

#   ifndef FLAG_RELEASE
    if (!newShader->is_compiled())
    {
        std::cerr << "Can't load shader program: \n"
                << "Vertex: " << vertexPath << "\n"
                << "Fragment: " << fragmentPath << std::endl;
        return nullptr;
    }
#   endif

    return newShader;
}

ShaderProgram *ResourceManager::getShaderProgram(const std::string &shaderName) const
{
    ShaderProgramsMap::const_iterator iter = _shaderProgramsMap.find(shaderName);

#   ifndef FLAG_RELEASE
    if (iter == _shaderProgramsMap.end())
    {
        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }
#   endif

    return iter->second.get();
}

Texture2D *ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePath, const unsigned char &number)
{
    int channels = 0, width = 0, height = 0;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *pixels = stbi_load((_executablePath + texturePath).c_str(), &width, &height, &channels, 0);

#   ifndef FLAG_RELEASE
    if (!pixels)
    {
        std::cerr << "Can't load image: " << texturePath << std::endl;
        return nullptr;
    }
#   endif

   Texture2D *newTexture2D = _textures2DMap.emplace(textureName, std::make_unique<Texture2D>(width, height, pixels, channels, number, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second.get();

    stbi_image_free(pixels);
    return newTexture2D;
}

Texture2D *ResourceManager::getTexture2D(const std::string &textureName) const
{
    Textures2DMap::const_iterator iter = _textures2DMap.find(textureName);

#   ifndef FLAG_RELEASE
    if (iter == _textures2DMap.end())
    {
        std::cerr << "Can't find the texture2D: " << textureName << std::endl;
        return nullptr;
    }
#   endif
    return iter->second.get();
}

Texture2D *ResourceManager::loadTextureAtlas(const std::string &textureName, const std::string &texturePath,
                                                           const std::vector<std::string> &subTextureNames, const unsigned int &subTextureWidth,
                                                           const unsigned int &subTextureHeight, const unsigned char &textureNumber)
{
    auto texture = loadTexture2D(std::move(textureName), std::move(texturePath), textureNumber);
    if (texture)
    {
        const unsigned int textureWidth = texture->get_width();
        const unsigned int textureHeight = texture->get_height();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;
        for (const auto &subTextureName : subTextureNames)
        {
            glm::vec2 leftBottom(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
            glm::vec2 rigthTop(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

            texture->add_sub_texture(subTextureName, leftBottom, rigthTop);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth)
            {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
    return texture;
}

Sound *ResourceManager::loadSound(const std::string &soundName, ma_engine *engine, const std::string &soundPath, const unsigned int &soundFlag)
{
    Sound *newSound= _soundsMap.emplace(soundName, std::make_unique<Sound>(engine, (_executablePath + soundPath).c_str(), soundFlag)).first->second.get();
    return newSound;
}

Sound *ResourceManager::getSound(const std::string &soundName) const
{
    SoundsMap::const_iterator iter = _soundsMap.find(soundName);

#ifndef FLAG_RELEASE
    if (iter == _soundsMap.end())
    {
        std::cerr << "Can't find the Sound: " << soundName << std::endl;
        return nullptr;
    }
#endif
    return iter->second.get();
}

