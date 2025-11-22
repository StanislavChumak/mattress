#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../external/jsonUtils.h"

typedef unsigned long EntityID;
class ResourceManager;

struct Animator {
    unsigned short currentFrame = 0;
    double currentAnimationTime = 0;
    unsigned short count = 0;
    unsigned short offset = 0;
    std::vector<double> durations;

    Animator() = default;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::vector<double> buffer;
        for(auto frameDuration : getVarJSON<simdjson::ondemand::array>(obj["durations"]))
        {
            durations.push_back(getVarJSON<double>(frameDuration.value()));
        }
        count = durations.size();
    }
};

#endif
