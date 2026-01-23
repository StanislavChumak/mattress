#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "../external/jsonUtils.h"

#include "glm/vec2.hpp"

#include <vector>
#include <memory>

class ResourceManager;
class Texture2D;

class TextureAtlas
{
public:
    struct SubTexture2D
    {
        glm::vec2 lbVertex = glm::vec2(0.0f);
        glm::vec2 rtVertex = glm::vec2(1.0f);
    };

private:
    std::vector<SubTexture2D> _atlas;

public:
    void fromJson(simdjson::ondemand::object obj, ResourceManager &resource);
    TextureAtlas() = default;
    TextureAtlas(const TextureAtlas &) = delete;
    TextureAtlas &operator=(const Texture2D &) = delete;
    TextureAtlas(TextureAtlas &&other) noexcept;
    TextureAtlas &operator=(TextureAtlas &&other) noexcept;
    ~TextureAtlas() = default;

    const SubTexture2D &getSubTexture(const size_t index) const;
};

#endif
