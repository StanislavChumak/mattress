#ifndef STATE_ANIMATOR_SYSTEM_H
#define STATE_ANIMATOR_SYSTEM_H

class ECSWorld;

class StateAnimatorSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};


#endif
