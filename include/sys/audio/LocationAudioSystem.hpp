#ifndef LOCATION_AUDIO_SYSTEM_HPP
#define LOCATION_AUDIO_SYSTEM_HPP

#include "sys/System.hpp"

namespace mtrs::sys
{

class LocationAudioSystem : public System<LocationAudioSystem>
{
public:
    static void update_imp(comp::ECSWorld &world, const double &delta);
    static constexpr int get_prioritet_imp() { return SystemPriority::AUDIO; };
    static constexpr const char *get_system_name_imp() { return "LocationAudioSystem"; }
};

}

#endif
