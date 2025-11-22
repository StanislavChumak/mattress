#ifndef STATE_ANIMATOR_H
#define STATE_ANIMATOR_H

#include "../external/jsonUtils.h"
#include <unordered_map>

struct StateAnimator
{
    std::unordered_map<std::string, std::pair<unsigned short, unsigned short>> states;
    std::string currentState = "";
    bool dirty = true;

    void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        for(auto field : obj)
        {
            std::string_view name = getResultJSON<std::string_view>(field.unescaped_key());
            simdjson::ondemand::object state = getVarJSON<simdjson::ondemand::object>(field.value());

            unsigned short offset = getVarJSON<int64_t>(state["offset"]);
            unsigned short count = getVarJSON<int64_t>(state["count"]);

            states.emplace(std::string(name), std::make_pair(offset, count));

            if(currentState == "")
                currentState = std::string(name);
        }
    }

    void setState(std::string state)
    {
        currentState = state;
        dirty = true;
    }
};


#endif
