#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

class ECSWorld;

class RenderSystem
{
public:
    RenderSystem() = default;

    static void update(ECSWorld &world, const double &delta);
};

#endif
