#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_H

class ECSWorld;

class CameraSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
