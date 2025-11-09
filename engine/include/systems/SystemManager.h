#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <unordered_set>
#include <unordered_map>
#include <functional>
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

class ECSWorld;

class SystemManager
{
private:
    std::unordered_set<std::string> _states;
    std::unordered_map<std::type_index, std::pair<int, std::function<void(ECSWorld&, const double&)>>> _updates;
    std::unordered_map<std::string, std::vector<std::type_index>> _stateUpdateMap;
    std::vector<std::type_index> _alwaysUpdates;

    void rebuildCache();
    std::vector<std::type_index> _cachedMergedUpdates;
    bool _cacheDirty;
    std::string _cacheState;
public:
    SystemManager();

    void update(ECSWorld &world, const double &delta, const std::string &currentState);

    bool addState(std::string state);

    template <typename T>
    void registerUpdete(int priority)
    {
        _updates.try_emplace(std::type_index(typeid(T)), std::make_pair<int, std::function<void(ECSWorld&, const double&)>>
            (std::move(priority), [](ECSWorld &world, const double &delta) { T::update(world, delta); }));
    }

    void setStateUpdates(std::string state , std::vector<std::type_index> &&systems);
    void addStateUpdate(std::string state , std::type_index system);

    template <typename T>
    void registerAlwaysUpdate()
    {
        _alwaysUpdates.push_back(std::type_index(typeid(T)));
    }
};

#endif
