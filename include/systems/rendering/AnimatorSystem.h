#ifndef ANIMATOR_SYSTEM_H
#define ANIMATOR_SYSTEM_H

class ECSWorld;

class AnimatorSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
