#ifndef GLOBAL_TRANSFORM_SYSTEM_H
#define GLOBAL_TRANSFORM_SYSTEM_H

class ECSWorld;

class GlobalTransformSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
