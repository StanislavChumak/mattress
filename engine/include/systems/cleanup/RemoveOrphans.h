#ifndef REMOVE_ORPHANS_H
#define REMOVE_ORPHANS_H

class ECSWorld;

class RemoveOrphans
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
