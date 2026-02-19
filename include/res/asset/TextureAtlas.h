#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "jsonUtils.h"

#include "glm/vec2.hpp"

#include <vector>

class ResourceManager;

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
    void from_json(simdjson::ondemand::object obj, ResourceManager &resource);
    TextureAtlas() = default;
    TextureAtlas(const TextureAtlas &) = delete;
    TextureAtlas &operator=(const TextureAtlas &) = delete;
    TextureAtlas(TextureAtlas &&other) noexcept;
    TextureAtlas &operator=(TextureAtlas &&other) noexcept;
    ~TextureAtlas() = default;

    const SubTexture2D &get_sub_texture(const size_t index) const;
};

#endif
