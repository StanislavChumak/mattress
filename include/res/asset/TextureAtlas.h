#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include "glm/vec2.hpp"

#include "res/asset/Asset.h"

#include <vector>

namespace mtrs::res
{

class ResourceManager;

class TextureAtlas : public Asset
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
    ASSETS_CONSTRUCTORS(TextureAtlas);

    std::string get_type_name() override;
    uint32_t get_type_size() override;

    const SubTexture2D &get_sub_texture(const size_t index) const;
};

}

#endif
