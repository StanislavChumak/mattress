#include "systems/SystemManager.h"

#include <algorithm>

SystemManager::SystemManager()
: _cacheDirty(true)
, _cacheState("")
{}

void SystemManager::rebuildCache()
{
    _cachedMergedSystems = _alwaysRunSystems;
    _cachedMergedSystems.reserve(_alwaysRunSystems.size() + _stateSystemsMap[_cacheState].size());

    _cachedMergedSystems.insert(_cachedMergedSystems.end(), _stateSystemsMap[_cacheState].begin(), _stateSystemsMap[_cacheState].end());

    std::sort(_cachedMergedSystems.begin(), _cachedMergedSystems.end(),
    [&](std::type_index a, std::type_index b) { return _systems[a].first > _systems[b].first; });
}

void SystemManager::update(ECSWorld &world, const double &delta, const std::string &currentState)
{
    if(_cacheDirty || _cacheState != currentState)
    {
        _cacheState = currentState;
        rebuildCache();
    }

    for(auto id : _cachedMergedSystems)
    {
        _systems[id].second->update(world, delta);
    }
}


bool SystemManager::addState(std::string state)
{
    _cacheDirty = true;
    return _states.insert(state).second;
}

void SystemManager::registerAlwaysRunSystem(std::type_index id)
{
    _alwaysRunSystems.push_back(id);
}

void SystemManager::setStateSystems(std::string state , std::vector<std::type_index> &&systems)
{
    _stateSystemsMap.try_emplace(std::move(state), std::move(systems));
}

void SystemManager::addStateSystem(std::string state , std::type_index system)
{
    _stateSystemsMap[state].push_back(system);
}