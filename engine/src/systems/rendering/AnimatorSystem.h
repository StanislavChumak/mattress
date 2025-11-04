#ifndef ANIMATOR_SYSTEM_H
#define ANIMATOR_SYSTEM_H

#include "SystemBase.h"

class AnimatorSystem : public SystemBase
{
public:
    AnimatorSystem() = default;

    void update(ECSWorld &world, const double &delta) override;
};

#endif
