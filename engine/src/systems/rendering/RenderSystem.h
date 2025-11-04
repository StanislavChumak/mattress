#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "SystemBase.h"

class RenderSystem : public SystemBase
{
public:
    RenderSystem() = default;

    void update(ECSWorld &world, const double &delta) override;
};

#endif
