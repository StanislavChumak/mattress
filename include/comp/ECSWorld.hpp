#ifndef ECSWORLD_HPP
#define ECSWORLD_HPP

#include "collections/View.hpp"
#include "collections/ComponentManager.hpp"
#include "util/type/prs/MtrsFileManager.hpp"

#include <unordered_map>
#include <stack>
#include <string>
#include <vector>

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
        uint64_t tmp_hash = 0;
        std::unordered_map<uint64_t, EntityID> local_entities;
        bool turn_on = false;
    };

    enum class SceneOp : uint8_t { Load, Remove, TurnOn, TurnOff };

    struct SceneOperation
    {
        SceneOp op = SceneOp::Load;
        uint64_t tmp_hash = 0;
        uint64_t scn_hash = 0;
    };

    std::unordered_map<uint64_t, std::string> _templates;
    std::unordered_map<uint64_t, Scene> _scenes;
    std::vector<SceneOperation> _pending_ops;

    std::string _scenes_dir;

    res::ResourceManager *_resources = nullptr;
    
    ComponentManager _components;
    prs::MtrsFileManager _file_manager;

    std::deque<EntityID> _destroy_ids;
    std::stack<EntityID> _freed_ids;

    prs::MtrsFileManager::MtrsFile *open(uint64_t tmp_hash);

    void do_load(uint64_t tmp_hash, uint64_t scn_hash);
    void do_remove(uint64_t scn_hash);
    void do_turn_on(uint64_t scn_hash);
    void do_turn_off(uint64_t scn_hash);

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

// API for engine
    void set_resources(res::ResourceManager *resources);

    void clear_all();
    void update(const double &delta);

    template<typename... Components>
    View<Components...> view()
    {
        return _components.view<Components...>();
    }

    template<typename Component, typename ...Args>
    Component* init_single_comp(Args&& ...args)
    {
        return _components.init_single_comp<Component>(args...);
    }

// API for script
    void load_scene(uint64_t tmp_hash, uint64_t scn_hash);
    void remove_scene(uint64_t scn_hash);
    void turn_on_scene(uint64_t scn_hash);
    void turn_off_scene(uint64_t scn_hash);

    void mark_destroy(EntityID entity);

    EntityID get_entity(uint64_t scn_hash, uint64_t ent_hash);

    void *single_comp(uint64_t comp_hash);
    void *component(uint64_t comp_hash, EntityID entity);

    bool save_static_to_file(uint64_t scn_hash, uint64_t ent_hash,
        uint64_t comp_hash, size_t field, void *data, size_t size);

    bool save_dynamic_to_file(uint64_t scn_hash, uint64_t ent_hash,
        uint64_t comp_hash, size_t field, void *data, size_t size);
};

}

#endif
