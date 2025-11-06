#include "resources/ResourceManager.h"

#ifndef FLAG_RELEASE
#include <iostream>
#endif

simdjson::simdjson_result<simdjson::ondemand::document>
ResourceManager::getJSON(const std::string& path, bool isResource)
{
    simdjson::ondemand::parser *parser = &_parser;
    if(isResource) parser = &_resourceParser;
    if(auto it = _jsonCache.find(path); it != _jsonCache.end())
        return parser->iterate(*it->second);

    std::unordered_map<std::string, std::string> defines;
    _jsonCache[path] = std::make_shared<simdjson::padded_string>(std::move(preprocessJSON(path, defines)));

    return parser->iterate(*_jsonCache[path]);
}

void ResourceManager::collectUnused() {
    for (auto it = _resourceCache.begin(); it != _resourceCache.end(); ) {
        if (it->second.expired()) {
#           ifndef FLAG_RELEASE
            std::cout << "Unloading resource: " << it->first << std::endl;
#           endif
            it = _resourceCache.erase(it);
        } else {
            ++it;
        }
    }
}


// ShaderProgram *ResourceManager::loadShaders(const std::string &resourseName, const std::string &vertexPath, const std::string &fragmentPath)
// {


//     ShaderProgram *newShader = _shaderProgramsMap.emplace(resourseName, std::make_unique<ShaderProgram>(vertexShaderCode.c_str(), fragmentShaderCode.c_str())).first->second.get();

// #   ifndef FLAG_RELEASE
//     if (!newShader->isCompiled())
//     {
//         std::cerr << "Can't load shader program: \n"
//                 << "Vertex: " << vertexPath << "\n"
//                 << "Fragment: " << fragmentPath << std::endl;
//         return nullptr;
//     }
// #   endif

//     return newShader;
// }



// Texture2D *ResourceManager::loadTexture2D(const std::string &textureName, const std::string &texturePath, const unsigned char &number)
// {
//     int channels = 0, width = 0, height = 0;

//     stbi_set_flip_vertically_on_load(true);
//     unsigned char *pixels = stbi_load((_executablePath + texturePath).c_str(), &width, &height, &channels, 0);

// #   ifndef FLAG_RELEASE
//     if (!pixels)
//     {
//         std::cerr << "Can't load image: " << texturePath << std::endl;
//         return nullptr;
//     }
// #   endif

//    Texture2D *newTexture2D = _textures2DMap.emplace(textureName, std::make_unique<Texture2D>(width, height, pixels, channels, number, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second.get();

//     stbi_image_free(pixels);
//     return newTexture2D;
// }

// Texture2D *ResourceManager::getTexture2D(const std::string &textureName) const
// {
//     Textures2DMap::const_iterator iter = _textures2DMap.find(textureName);

// #   ifndef FLAG_RELEASE
//     if (iter == _textures2DMap.end())
//     {
//         std::cerr << "Can't find the texture2D: " << textureName << std::endl;
//         return nullptr;
//     }
// #   endif
//     return iter->second.get();
// }

// Texture2D *ResourceManager::loadTextureAtlas(const std::string &textureName, const std::vector<std::string> &subTextureNames, const unsigned int &subTextureWidth,
//                                             const unsigned int &subTextureHeight, const unsigned char &textureNumber)
// {
//     auto texture = loadTexture2D(std::move(textureName), std::move(texturePath), textureNumber);
//     if (texture)
//     {
//         const unsigned int textureWidth = texture->get_width();
//         const unsigned int textureHeight = texture->get_height();
        
//     }
//     return texture;
// }

// Sound *ResourceManager::loadSound(const std::string &soundName, ma_engine *engine, const std::string &soundPath, const unsigned int &soundFlag)
// {
//     Sound *newSound= _soundsMap.emplace(soundName, std::make_unique<Sound>(engine, (_executablePath + soundPath).c_str(), soundFlag)).first->second.get();
//     return newSound;
// }

// Sound *ResourceManager::getSound(const std::string &soundName) const
// {
//     SoundsMap::const_iterator iter = _soundsMap.find(soundName);

// #ifndef FLAG_RELEASE
//     if (iter == _soundsMap.end())
//     {
//         std::cerr << "Can't find the Sound: " << soundName << std::endl;
//         return nullptr;
//     }
// #endif
//     return iter->second.get();
// }

