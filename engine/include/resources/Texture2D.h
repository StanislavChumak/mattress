#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <glm/vec2.hpp>

#include "../external/jsonUtils.h"

class Texture2D
{
public:

    struct SubTexture2D
    {
        glm::vec2 lbVertex = glm::vec2(0.0f);
        glm::vec2 rtVertex = glm::vec2(1.0f);
    };

    Texture2D() = default;
    void fromJson(simdjson::ondemand::object obj);
    Texture2D(Texture2D &) = delete;
    Texture2D &operator=(const Texture2D &) = delete;
    Texture2D(Texture2D &&other) noexcept;
    Texture2D &operator=(Texture2D &&other) noexcept;
    ~Texture2D();

    void loadAtlas(const size_t count, const unsigned int &subTextureWidth, const unsigned int &subTextureHeight);
    const SubTexture2D& getSubTexture(const size_t index) const;
    void bind() const;
    void active() const;
    unsigned int id() const noexcept;
private:

    unsigned int _ID;
    unsigned char _number;
    int _mode;
    int _width;
    int _height;

    std::vector<SubTexture2D> _atlas;
};


#endif
