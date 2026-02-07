#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

class ECSWorld;

class AudioSystem
{
public:
    AudioSystem() = default;

    static void update(ECSWorld &world, const double &delta);
};

#endif
