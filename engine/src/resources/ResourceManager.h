#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <glm/fwd.hpp>

#include "renderer/ShaderProgram.h"
#include "renderer/Texture2D.h"
#include "sound/Sound.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

class ShaderProgram;
class Texture2D;
class Sound;

typedef std::unordered_map<std::string, std::unique_ptr<ShaderProgram>> ShaderProgramsMap;
typedef std::unordered_map<std::string, std::unique_ptr<Texture2D>> Textures2DMap;
typedef std::unordered_map<std::string, std::unique_ptr<Sound>> SoundsMap;

class ResourceManager
{
private:
    ShaderProgramsMap _shaderProgramsMap;
    Textures2DMap _textures2DMap;
    SoundsMap _soundsMap;
    
    std::string _executablePath;

    std::string getFileString(const std::string &relativeFilePath);
public:
    ResourceManager();
    ~ResourceManager();
    void clear();

    void setExecutablePath(const std::string &executablePath);

    ShaderProgram *loadShaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath);
    ShaderProgram *getShaderProgram(const std::string &shaderName) const;

    Texture2D *loadTexture2D(const std::string &textureName, const std::string &texturePath, const unsigned char &textureNumber = 0);
    Texture2D *getTexture2D(const std::string &textureName) const;
    Texture2D *loadTextureAtlas(const std::string &textureName, const std::string &texturePath, const std::vector<std::string> &subTextureNames,
                                              const unsigned int &subTextureWidth, const unsigned int &subTextureHeight, const unsigned char &textureNumber = 0);

    Sound *loadSound(const std::string &soundName, ma_engine *engine, const std::string &soundPath, const unsigned int &soundFlag);
    Sound *getSound(const std::string &soundName) const;
};

#endif
