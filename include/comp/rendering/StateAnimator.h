#ifndef STATE_ANIMATOR_H
#define STATE_ANIMATOR_H

#include "jsonUtils.h"
#include <unordered_map>

struct StateAnimator
{
    std::unordered_map<std::string, std::pair<unsigned short, unsigned short>> states;
    std::string currentState = "";
    bool dirty = true;

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        for(auto field : obj)
        {
            std::string_view name = get_result_json<std::string_view>(field.unescaped_key());
            simdjson::ondemand::object state = get_var_json<simdjson::ondemand::object>(field.value());

            unsigned short offset = get_var_json<int64_t>(state["offset"]);
            unsigned short count = get_var_json<int64_t>(state["count"]);

            states.emplace(std::string(name), std::make_pair(offset, count));

            if(currentState == "")
                currentState = std::string(name);
        }
    }

    void set_state(std::string state)
    {
        currentState = state;
        dirty = true;
    }
};


#endif
