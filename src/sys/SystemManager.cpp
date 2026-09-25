#include "sys/SystemManager.hpp"

#include "sys/core/InputSystem.hpp"
#include "sys/core/InputSybscrubersSystem.hpp"
#include "sys/core/ScriptsSystem.hpp"
#include "sys/core/TransformSystem.hpp"

#include "sys/rendering/RenderSystem.hpp"
#include "sys/rendering/CameraSystem.hpp"
#include "sys/rendering/SpriteSubmitSystem.hpp"
#include "sys/rendering/AnimatorSystem.hpp"
#include "sys/rendering/StateAnimatorSystem.hpp"
#include "sys/rendering/SpriteMapSubmitSystem.hpp"
#include "sys/rendering/MapAnimatorSystem.hpp"
#include "sys/rendering/TextSubmitSystem.hpp"

#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_message.hpp"

#include <algorithm>

namespace mtrs::sys
{

SystemManager::SystemManager()
: _cache_dirty(true)
{
#define UPDATE_SYSTEM(System) { math::hash64(System::get_system_name()), \
    {System::get_prioritet(), System::update} }
    
    _updates = {
        UPDATE_SYSTEM(InputSystem),
        UPDATE_SYSTEM(InputSybscrubersSystem),
        UPDATE_SYSTEM(ScriptsSystem),
        UPDATE_SYSTEM(TransformSystem),

        UPDATE_SYSTEM(RenderSystem),
        UPDATE_SYSTEM(CameraSystem),
        UPDATE_SYSTEM(SpriteSubmitSystem),
        UPDATE_SYSTEM(AnimatorSystem),
        UPDATE_SYSTEM(StateAnimatorSystem),
        UPDATE_SYSTEM(SpriteMapSubmitSystem),
        UPDATE_SYSTEM(MapAnimatorSystem),
        UPDATE_SYSTEM(TextSubmitSystem)
    };
#undef UPDATE_SYSTEM
}

void SystemManager::rebuild_cache()
{
    _cached_updates.clear();
    _cached_updates.reserve(_updates.size() - _disabled_systems.size());

    for (const auto &[hash, _] : _updates)
    {
        if (_disabled_systems.find(hash) == _disabled_systems.end())
            _cached_updates.push_back(hash);
    }

    std::sort(_cached_updates.begin(), _cached_updates.end(),
        [&](uint64_t a, uint64_t b)
        {
            return _updates[a].first < _updates[b].first;
        });
}

void SystemManager::update(comp::ECSWorld &world, const double &delta)
{
    if(_cache_dirty)
    {
        rebuild_cache();
    }

    for(auto id : _cached_updates)
    {
        _updates[id].second(world, delta);
    }
}

void SystemManager::turn_on_system(uint64_t hash_system)
{
    auto iter = _disabled_systems.find(hash_system);
    if(iter == _disabled_systems.end()) return;
    _disabled_systems.erase(iter);
}

void SystemManager::turn_off_system(uint64_t hash_system)
{
    auto iter = _disabled_systems.find(hash_system);
    if(iter != _disabled_systems.end()) return;
    _disabled_systems.emplace(hash_system);
}

bool SystemManager::is_turn_on_system(uint64_t hash_system)
{
    return _disabled_systems.find(hash_system) == _disabled_systems.end();
}

}