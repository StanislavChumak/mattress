#ifndef LOCATION_AUDIO_SYSTEM_H
#define LOCATION_AUDIO_SYSTEM_H

class ECSWorld;

class LocationAudioSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

#endif
