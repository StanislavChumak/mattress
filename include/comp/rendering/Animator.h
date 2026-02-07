#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "jsonUtils.h"

typedef unsigned long EntityID;
class ResourceManager;

struct Animator {
    unsigned short currentFrame = 0;
    double currentAnimationTime = 0;
    unsigned short count = 0;
    unsigned short offset = 0;
    std::vector<double> durations;

    Animator() = default;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::vector<double> buffer;
        for(auto frameDuration : get_var_json<simdjson::ondemand::array>(obj["durations"]))
        {
            durations.push_back(get_var_json<double>(frameDuration.value()));
        }
        count = durations.size();
    }
};

#endif
