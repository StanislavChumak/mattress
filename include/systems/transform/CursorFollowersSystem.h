#ifndef CURSOR_FOLLOWERS_SYSTEM_H
#define CURSOR_FOLLOWERS_SYSTEM_H

class ECSWorld;

class CursorFollowersSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
