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

    std::vector<std::function<void(EntityID)>> _removeEntyti;
    void removeEntyti(EntityID id) { for(auto fun : _removeEntyti) fun(id); }
    std::vector<EntityID> _destroyQueue;

    template<typename Component>
    struct ComponentPool
    {
        SparseSet<Component> sparseSet;
        bool initialized = false;
    };

    template<typename Component>
    ComponentPool<Component>& getPool()
    {
        static ComponentPool<Component> pool;
        if(!pool.initialized)
        {
            _removeEntyti.push_back([&](EntityID id){ pool.sparseSet.remove(id);});
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
    SingleComponentPool<Component>& getSinglePool()
    {
        static SingleComponentPool<Component> pool;
        if(!pool.initialized)
            _clearSingletons.push_back([&](){ pool.component.reset();});
        return pool;
    }

    EntityID nextEntity = 0;
    std::unordered_map<std::string, EntityID> namesEntity;

public:

    EntityID createEntity() { return nextEntity++; }
    void naming(EntityID id, std::string name) { namesEntity.emplace(name, id); }
    void renaming(std::string name) { namesEntity.erase(name); }
    EntityID getNamedEntity(std::string name)
    { 
        auto it = namesEntity.find(name);
        if(it != namesEntity.end())
            return it->second;
#ifndef FLAG_RELEASE
        std::cerr << "Fatal find Entity to name: " << name << std::endl;
#endif
        return NULL_ENTITY;
    }

    void clearSets() { for(auto fun : _clearSets) fun(); }
    void clearSingletons() { for(auto fun : _clearSingletons) fun(); }

    void markDestroy(EntityID id) { _destroyQueue.push_back(id); }
    void removeMarked() { for(auto id : _destroyQueue) {removeEntyti(id); }; _destroyQueue.clear(); }

    template<typename Component> SparseSet<Component>& getSets();
    template<typename Component> void clearSets();
    template<typename Component> bool hasSets();

    template<typename Component> Component *getComponent(EntityID entity);
    template<typename Component> Component &addComponent(EntityID entity, Component component);
    template<typename Component> void removeComponent(EntityID entity);
    template<typename Component> bool hasComponent(EntityID entity);

    template<typename Component> Component *getSingleComponent();
    template<typename Component> Component &addSingleComponent(Component component);
    template<typename Component> bool hasSingleComponent();
    template<typename Component> void removeSingleComponent();
    
    template<typename... Component>
    View<Component...> view() {
        return View<Component...>(getSets<Component>()...);
    }
};

template<typename Component>
SparseSet<Component>& ECSWorld::getSets()
{
    return getPool<Component>().sparseSet;
}

template<typename Component>
void ECSWorld::clearSets()
{
    getSets<Component>().clear();
}

template<typename Component>
bool ECSWorld::hasSets()
{
    return getPool<Component>().initialized;
}


template<typename Component>
Component *ECSWorld::getComponent(EntityID entity)
{
    return getSets<Component>().get(entity);
}

template<typename Component>
Component& ECSWorld::addComponent(EntityID entity, Component component)
{
    return getSets<Component>().add(entity, std::move(component));
}

template<typename Component>
void ECSWorld::removeComponent(EntityID entity)
{
    getSets<Component>().remove(entity);
}

template<typename Component>
bool ECSWorld::hasComponent(EntityID entity)
{
    return getSets<Component>().has(entity);
}


template<typename Component>
Component *ECSWorld::getSingleComponent()
{
    auto& pool = getSinglePool<Component>();
    if (!pool.initialized) {
        return nullptr;
    }
    return pool.component.get();
}

template<typename Component>
Component& ECSWorld::addSingleComponent(Component component)
{
    auto& pool = getSinglePool<Component>();
    pool.component = std::make_unique<Component>(std::move(component));
    pool.initialized = true;
    return *pool.component;
}

template<typename Component>
bool ECSWorld::hasSingleComponent()
{
    return getSinglePool<Component>().initialized;
}

template<typename Component>
void ECSWorld::removeSingleComponent()
{
    auto& pool = getSinglePool<Component>();
    if (pool.initialized) {
        pool.component.reset();
        pool.initialized = false;
    }
}


#endif
