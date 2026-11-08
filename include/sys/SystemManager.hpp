#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include "System.hpp"
#include "util/fun/math/hash.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

namespace mtrs::sys
{

class SystemManager
{
    using Update = void(*)(comp::ECSWorld&, const double&);
    std::unordered_map<uint64_t, std::pair<int, Update>> _updates;
    std::unordered_set<uint64_t> _disabled_systems;

    void rebuild_cache();
    std::vector<uint64_t> _cached_updates;
    bool _cache_dirty;

public:
    SystemManager();

    void update(comp::ECSWorld &world, const double &delta);

    void turn_on_system(uint64_t hash_system);
    void turn_off_system(uint64_t hash_system);
    bool is_turn_on_system(uint64_t hash_system);

    template<typename System> void register_update()
    {
        _updates[HASH64S(System::get_system_name())] = {System::get_prioritet(), System::update};
        _cache_dirty = true;
    }
};

}

#endif
