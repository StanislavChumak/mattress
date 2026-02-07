#ifndef SPRITE_RENDER_SYSTEM_H
#define SPRITE_RENDER_SYSTEM_H

#include <memory>

class ECSWorld;
class RenderContext;

class SpriteRenderSystem
{
public:
    static std::shared_ptr<RenderContext> context;
    static void update(ECSWorld &world, const double &delta);
};

#endif
