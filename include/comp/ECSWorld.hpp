#ifndef ECSWORLD_HPP
#define ECSWORLD_HPP

#include "collections/View.hpp"
#include "collections/ComponentManager.hpp"
#include "util/type/prs/MtrsFileManager.hpp"

#include <unordered_map>
#include <stack>
#include <string>

namespace mtrs::res
{
    class ResourceManager;
}

namespace mtrs::comp
{

class ECSWorld
{
    struct Scene
    {
        std::unordered_map<uint64_t, EntityID> local_entities;
        bool init;
        bool turn_on;
    };
    std::unordered_map<std::string, Scene> _scenes;

    std::string _scenes_dir;
    
    ComponentManager _components;
    prs::MtrsFileManager _file_manager;

    std::deque<EntityID> _destroy_ids;
    std::stack<EntityID> _freed_ids;

    prs::MtrsFileManager::MtrsFile *open_scene(decltype(_scenes)::iterator scene);

    ECSWorld(std::string scenes_dir, std::unordered_set<std::string> paths,
        uint64_t limit_size_cache);

public:
    ECSWorld(std::string scenes_dir, uint64_t limit_size_cache);
    
    ECSWorld() = delete;
    ECSWorld(ECSWorld &) = delete;
    ECSWorld &operator=(const ECSWorld &) = delete;
    ECSWorld(ECSWorld &&other) noexcept;
    ECSWorld &operator=(ECSWorld &&other) noexcept;
    ~ECSWorld();

    void load_scene(std::string scene, mtrs::res::ResourceManager& resource, bool is_turn_on = true);
    void remove_scene(std::string scene);
    void turn_on_scene(std::string scene);
    void turn_off_scene(std::string scene);

    void mark_destroy(EntityID entity);

    void clear_all();
    void update(const double &delta);

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }

    template<typename Component>
    Component* single_comp()
    {
        return _components.get_single_comp<Component>();
    }

    template<typename Component, typename ...Args>
    Component* single_comp(Args&& ...args)
    {
        return _components.add_single_comp<Component>(args...);
    }

    void *single_comp(uint64_t hash_comp);

    template<typename Component>
    Component *component(EntityID entity)
    {
        return _components.get_comp<Component>(entity);
    }

    void *component(uint64_t hash_comp, EntityID entity);

    EntityID get_entity(const char *scene, uint64_t hash_entity);

    bool save_static_to_file(const char *scene, uint64_t hash_entity,
        uint64_t hash_comp, size_t field, void *data, size_t size);

    bool save_dynamic_to_file(const char *scene, uint64_t hash_entity,
        uint64_t hash_comp, size_t field, void *data, size_t size);
};

}

#endif
