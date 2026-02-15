#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "jsonUtils.h"

typedef unsigned long EntityID;
class ResourceManager;

struct Animator {
    size_t countFrame = 0;
    std::vector<float> durations;

    size_t currentFrame = 0;
    float currentAnimationTime = 0;
    size_t offset = 0;

    Animator() = default;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        std::vector<double> buffer;
        for(auto frameDuration : get_var_json<simdjson::ondemand::array>(obj["durations"]))
        {
            durations.push_back(get_var_json<double>(frameDuration.value()));
        }
        countFrame = durations.size();
    }
};

#endif
