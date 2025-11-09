#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glm/vec2.hpp>

#include "../external/jsonUtils.h"

#include <unordered_map>
#include <vector>
#include <string>


class Texture2D
{
public:
    struct SubTexture2D
    {
        glm::vec2 leftBottomVertex = glm::vec2(0.0f);
        glm::vec2 rightTopVertex = glm::vec2(1.0f);
    };

    Texture2D() = default;
    void fromJson(simdjson::ondemand::object obj);
    Texture2D(Texture2D &) = delete;
    Texture2D &operator=(const Texture2D &) = delete;
    Texture2D &operator=(Texture2D &&Texture2D);
    Texture2D(Texture2D &&Texture2D);
    ~Texture2D();

    void loadAtlas(const std::vector<std::string> &subTextureNames, const unsigned int &subTextureWidth, const unsigned int &subTextureHeight);
    const SubTexture2D& getSubTexture(const std::string &subTextureName) const;
    void bind() const;
    void active() const;

    

private:
    unsigned int _ID;
    unsigned char _number;
    int _mode;
    int _width;
    int _height;

    std::unordered_map<std::string, SubTexture2D> _atlas;
};


#endif
