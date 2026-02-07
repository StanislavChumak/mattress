#include "sys/SystemManager.h"

#include <algorithm>

SystemManager::SystemManager()
: _cacheDirty(true)
, _cacheState("")
{}

void SystemManager::rebuild_cache()
{
    _cachedMergedUpdates = _alwaysUpdates;
    _cachedMergedUpdates.reserve(_alwaysUpdates.size() + _stateUpdateMap[_cacheState].size());

    _cachedMergedUpdates.insert(_cachedMergedUpdates.end(), _stateUpdateMap[_cacheState].begin(), _stateUpdateMap[_cacheState].end());

    std::sort(_cachedMergedUpdates.begin(), _cachedMergedUpdates.end(),
    [&](std::type_index a, std::type_index b) { return _updates[a].first > _updates[b].first; });
}

void SystemManager::update(ECSWorld &world, const double &delta, const std::string &currentState)
{
    if(_cacheDirty || _cacheState != currentState)
    {
        _cacheState = currentState;
        rebuild_cache();
    }

    for(auto id : _cachedMergedUpdates)
    {
        _updates[id].second(world, delta);
    }
}


bool SystemManager::add_state(std::string state)
{
    _cacheDirty = true;
    return _states.insert(state).second;
}

void SystemManager::set_state_updates(std::string state , std::vector<std::type_index> &&systems)
{
    _stateUpdateMap.try_emplace(std::move(state), std::move(systems));
}

void SystemManager::add_state_update(std::string state , std::type_index system)
{
    _stateUpdateMap[state].push_back(system);
}