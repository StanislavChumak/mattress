#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include "SparseSet.hpp"
#include "View.hpp"

#include <unordered_set>
#include <memory>

namespace mtrs::comp
{

class ComponentManager
{
    EntityID _entity_index = 0;
    std::unordered_set<EntityID> _disabled_ids;

    template<typename Component>
    struct ComponentPool
    {
        SparseSet<Component> sparse_set;
    };

    template<typename Component>
    ComponentPool<Component>& get_pool()
    {
        static ComponentPool<Component> pool;
        return pool;
    }

    template<typename Component>
    struct SingleComponentPool
    {
        std::unique_ptr<Component> component = nullptr;
    };

    template<typename Component>
    SingleComponentPool<Component>& get_single_pool()
    {
        static SingleComponentPool<Component> pool;
        return pool;
    }

public:
    ComponentManager() = default;
    ComponentManager(ComponentManager &) = delete;
    ComponentManager &operator=(const ComponentManager &) = delete;
    ComponentManager(ComponentManager &&other) noexcept;
    ComponentManager &operator=(ComponentManager &&other) noexcept;
    ~ComponentManager();

    EntityID create_entity();
    
    void turn_on(EntityID entity);
    void turn_off(EntityID entity);
    void reserve_turn_off(size_t count);
    bool is_turn_on(EntityID entity);

    template<typename Component> SparseSet<Component>& get_set();
    template<typename Component> void clear_set();
    template<typename Component> bool has_set();

    template<typename Component> Component *get_comp(EntityID entity);
    template<typename Component, typename ...Args> Component *add_comp(EntityID entity, Args&& ...args);
    template<typename Component> void remove_comp(EntityID entity);
    template<typename Component> bool has_comp(EntityID entity);

    template<typename Component> Component *get_single_comp();
    template<typename Component, typename ...Args> Component *init_single_comp(Args&& ...args);
    template<typename Component> void remove_single_comp();
    template<typename Component> bool has_single_comp();

    template<typename... Components>
    View<Components...> view()
    {
        return View<Components...>(_disabled_ids, get_set<Components>()...);
    }
};

template<typename Component>
SparseSet<Component>& ComponentManager::get_set()
{
    return get_pool<Component>().sparse_set;
}

template<typename Component>
void ComponentManager::clear_set()
{
    get_set<Component>().clear();
}

template<typename Component>
bool ComponentManager::has_set()
{
    return get_pool<Component>().initialized;
}

template<typename Component>
Component *ComponentManager::get_comp(EntityID entity)
{
    return get_set<Component>().get(entity);
}

template<typename Component, typename ...Args>
Component *ComponentManager::add_comp(EntityID entity, Args&& ...args)
{
    return get_set<Component>().add(entity, args...);
}

template<typename Component>
void ComponentManager::remove_comp(EntityID entity)
{
    get_set<Component>().remove(entity);
}

template<typename Component>
bool ComponentManager::has_comp(EntityID entity)
{
    return get_set<Component>().has(entity);
}

template<typename Component>
Component *ComponentManager::get_single_comp()
{
    auto& pool = get_single_pool<Component>();
    return pool.component.get();
}

template<typename Component, typename ...Args>
Component *ComponentManager::init_single_comp(Args&& ...args)
{
    auto& pool = get_single_pool<Component>();
    pool.component = std::make_unique<Component>(args...);
    return pool.component.get();
}

template<typename Component>
void ComponentManager::remove_single_comp()
{
    auto& pool = get_single_pool<Component>();
    pool.component.reset();
}

template<typename Component>
bool ComponentManager::has_single_comp()
{
    return get_single_pool<Component>().component;
}

}

#endif
