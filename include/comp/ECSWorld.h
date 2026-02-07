#ifndef ECSWORLD_H
#define ECSWORLD_H

#include "SparseSet.h"
#include "View.h"

#include <functional>
#include <vector>
#include <memory>

#include <unordered_map>
#include <string>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

class ECSWorld
{
private:
    std::vector<std::function<void()>> _clearSets;
    std::vector<std::function<void()>> _clearSingletons;

    std::vector<std::function<void(EntityID)>> _removeEntity;
    void remove_entity(EntityID id) { for(auto fun : _removeEntity) fun(id); }
    std::vector<EntityID> _destroyQueue;

    template<typename Component>
    struct ComponentPool
    {
        SparseSet<Component> sparseSet;
        bool initialized = false;
    };

    template<typename Component>
    ComponentPool<Component>& get_pool()
    {
        static ComponentPool<Component> pool;
        if(!pool.initialized)
        {
            _removeEntity.push_back([&](EntityID id){ pool.sparseSet.remove(id);});
            _clearSets.push_back([&](){ pool.sparseSet.clear();});
            pool.initialized = true;
        }
        return pool;
    }

    template<typename Component>
    struct SingleComponentPool
    {
        std::unique_ptr<Component> component;
        bool initialized = false;
    };

    template<typename Component>
    SingleComponentPool<Component>& get_single_pool()
    {
        static SingleComponentPool<Component> pool;
        if(!pool.initialized)
            _clearSingletons.push_back([&](){ pool.component.reset();});
        return pool;
    }

    EntityID nextEntity = 0;
    std::unordered_map<std::string, EntityID> namesEntity;

public:

    EntityID create_entity() { return nextEntity++; }
    void naming(EntityID id, std::string name) { namesEntity.emplace(name, id); }
    void renaming(std::string name) { namesEntity.erase(name); }
    EntityID get_name_entity(std::string name)
    { 
        auto it = namesEntity.find(name);
        if(it != namesEntity.end())
            return it->second;
#ifndef FLAG_RELEASE
        std::cerr << "Fatal find Entity to name: " << name << std::endl;
#endif
        return NULL_ENTITY;
    }

    void clear_sets() { for(auto fun : _clearSets) fun(); }
    void clear_singletons() { for(auto fun : _clearSingletons) fun(); }

    void mark_destroy(EntityID id) { _destroyQueue.push_back(id); }
    void remove_marked() { for(auto id : _destroyQueue) {remove_entity(id); }; _destroyQueue.clear(); }

    template<typename Component> SparseSet<Component>& get_sets();
    template<typename Component> void clear_sets();
    template<typename Component> bool has_sets();

    template<typename Component> Component *get_comp(EntityID entity);
    template<typename Component> Component &add_comp(EntityID entity, Component component);
    template<typename Component> void remove_comp(EntityID entity);
    template<typename Component> bool has_component(EntityID entity);

    template<typename Component> Component *get_single_comp();
    template<typename Component> Component &add_single_comp(Component component);
    template<typename Component> bool has_single_comp();
    template<typename Component> void remove_single_comp();
    
    template<typename... Component>
    View<Component...> view() {
        return View<Component...>(get_sets<Component>()...);
    }
};

template<typename Component>
SparseSet<Component>& ECSWorld::get_sets()
{
    return get_pool<Component>().sparseSet;
}

template<typename Component>
void ECSWorld::clear_sets()
{
    get_sets<Component>().clear();
}

template<typename Component>
bool ECSWorld::has_sets()
{
    return get_pool<Component>().initialized;
}

template<typename Component>
Component *ECSWorld::get_comp(EntityID entity)
{
    return get_sets<Component>().get(entity);
}

template<typename Component>
Component& ECSWorld::add_comp(EntityID entity, Component component)
{
    return get_sets<Component>().add(entity, std::move(component));
}

template<typename Component>
void ECSWorld::remove_comp(EntityID entity)
{
    get_sets<Component>().remove(entity);
}

template<typename Component>
bool ECSWorld::has_component(EntityID entity)
{
    return get_sets<Component>().has(entity);
}


template<typename Component>
Component *ECSWorld::get_single_comp()
{
    auto& pool = get_single_pool<Component>();
    if (!pool.initialized) {
        return nullptr;
    }
    return pool.component.get();
}

template<typename Component>
Component& ECSWorld::add_single_comp(Component component)
{
    auto& pool = get_single_pool<Component>();
    pool.component = std::make_unique<Component>(std::move(component));
    pool.initialized = true;
    return *pool.component;
}

template<typename Component>
bool ECSWorld::has_single_comp()
{
    return get_single_pool<Component>().initialized;
}

template<typename Component>
void ECSWorld::remove_single_comp()
{
    auto& pool = get_single_pool<Component>();
    if (pool.initialized) {
        pool.component.reset();
        pool.initialized = false;
    }
}


#endif
