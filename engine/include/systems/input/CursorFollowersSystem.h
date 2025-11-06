#ifndef CURSOR_FOLLOWERS_SYSTEM_H
#define CURSOR_FOLLOWERS_SYSTEM_H

#include "SystemBase.h"

class ECSWorld;

class CursorFollowersSystem : public SystemBase
{
public:
    CursorFollowersSystem() = default;

    void update(ECSWorld &world, const double &delta) override;
};

#endif
