#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <typeindex>

namespace SystemPriority
{
    constexpr int INPUT = 10;           // введение
    constexpr int NETWORK = 20;         // Сетевая синхронизация
    constexpr int GAMEPLAY_EARLY = 30;  // Ранняя игровая логика
    constexpr int AI_PLANNING = 40;     // Планирование ИИ
    constexpr int PLAYER_CONTROL = 50;  // Управление игроком
    constexpr int AI_EXECUTION = 60;    // Исполнение ИИ
    constexpr int PHYSICS = 70;         // Физика
    constexpr int TRANSFORM = 80;       // вычесление позиций
    constexpr int COLLISION = 90;       // Обработка столкновений
    constexpr int ANIMATION = 100;      // Анимации
    constexpr int AUDIO = 110;          // Звук
    constexpr int UI_LOGIC = 120;       // Логика UI
    constexpr int RENDERING = 200;      // Рендеринг
    constexpr int CLEANUP = 1000;       // Очистка
}

class ECSWorld;

class SystemManager
{
private:
    std::unordered_set<std::string> _states;
    std::unordered_map<std::type_index, std::pair<int, void(*)(ECSWorld&, const double&)>> _updates;
    std::unordered_map<std::string, std::vector<std::type_index>> _stateUpdateMap;
    std::vector<std::type_index> _alwaysUpdates; 

    void rebuild_cache();
    std::vector<std::type_index> _cachedMergedUpdates;
    bool _cacheDirty;
    std::string _cacheState;
public:
    SystemManager();

    void update(ECSWorld &world, const double &delta, const std::string &currentState);

    bool add_state(std::string state);

    template <typename T>
    void register_updete(int priority)
    {
        _updates.try_emplace(std::type_index(typeid(T)), std::make_pair<int, void(*)(ECSWorld&, const double&)>(std::move(priority), T::update));
    }

    void set_state_updates(std::string state , std::vector<std::type_index> &&systems);
    void add_state_update(std::string state , std::type_index system);

    template <typename T>
    void register_always_update()
    {
        _alwaysUpdates.push_back(std::type_index(typeid(T)));
    }
};

#endif
