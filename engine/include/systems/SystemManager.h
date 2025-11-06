#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "../../include/SystemBase.h"

#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <typeindex>

namespace SystemPriority {
    constexpr int INPUT = 10;           // Ввод (самый первый)
    constexpr int NETWORK = 20;         // Сетевая синхронизация
    constexpr int GAMEPLAY_EARLY = 30;  // Ранняя игровая логика
    constexpr int AI_PLANNING = 40;     // Планирование ИИ
    constexpr int PLAYER_CONTROL = 50;  // Управление игроком
    constexpr int AI_EXECUTION = 60;    // Исполнение ИИ
    constexpr int PHYSICS = 75;         // Физика
    constexpr int COLLISION = 80;       // Обработка столкновений
    constexpr int ANIMATION = 90;       // Анимации
    constexpr int AUDIO = 100;          // Звук
    constexpr int UI_LOGIC = 120;       // Логика UI
    constexpr int RENDERING = 200;      // Рендеринг
    constexpr int CLEANUP = 1000;       // Очистка (самый последний)
}

class SystemManager
{
private:
    std::unordered_set<std::string> _states;
    std::unordered_map<std::type_index, std::pair<int, std::unique_ptr<SystemBase>>> _systems;
    std::unordered_map<std::string, std::vector<std::type_index>> _stateSystemsMap;
    std::vector<std::type_index> _alwaysRunSystems;

    void rebuildCache();
    std::vector<std::type_index> _cachedMergedSystems;
    bool _cacheDirty;
    std::string _cacheState;
public:
    SystemManager();

    void update(ECSWorld &world, const double &delta, const std::string &currentState);

    bool addState(std::string state);

    template <typename T, typename ...Args>
    void registerSystem(int priority, Args& ...args)
    {
        _systems.try_emplace(std::type_index(typeid(T)), std::make_pair(priority, std::make_unique<T>(args...)));
    }

    void setStateSystems(std::string state , std::vector<std::type_index> &&systems);
    void addStateSystem(std::string state , std::type_index system);

    void registerAlwaysRunSystem(std::type_index id);
    template <typename T>
    void registerAlwaysRunSystem()
    {
        _alwaysRunSystems.push_back(std::type_index(typeid(T)));
    }
};

#endif
