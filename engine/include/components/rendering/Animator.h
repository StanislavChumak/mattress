#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../external/jsonUtils.h"

typedef unsigned long EntityID;
class ResourceManager;

struct Animator {
    unsigned short currentFrame = 0;
    double currentAnimationTime = 0;
    unsigned short framesCount = 0;
    std::vector<double> framesDuration;

    Animator() = default;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::vector<double> buffer;
        for(auto frameDuration : getVarJSON<simdjson::ondemand::array>(obj["framesDuration"]))
        {
            framesDuration.push_back(getVarJSON<double>(frameDuration.value()));
        }
        framesCount = framesDuration.size();
    }
};

#endif
