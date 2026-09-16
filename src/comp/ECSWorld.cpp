#include "comp/ECSWorld.hpp"

#include "res/ResourceManager.hpp"

#include "comp/core/ScriptUpdate.hpp"
#include "comp/core/ScriptCallback.hpp"
#include "comp/core/StoredData.hpp"
#include "comp/core/Transform.hpp"

#include "comp/rendering/Sprite.hpp"
#include "comp/rendering/Animator.hpp"
#include "comp/rendering/StateAnimator.hpp"
#include "comp/rendering/SpriteMap.hpp"
#include "comp/rendering/MapAnimator.hpp"

#include "comp/ui/Label.hpp"

#include "comp/audio/SoundPlayer.hpp"

#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/States.hpp"
#include "comp/single/Render.hpp"
#include "comp/single/GlyphDecoder.hpp"
#include "comp/single/Audio.hpp"
#include "comp/single/Listener.hpp"
#include "comp/single/Cursor.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"

#include "util/fun/fs/get_folder.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/prs/read.hpp"

#include "util/type/prs/comp/comp_types.hpp"

#define SINGLE_COMPONENT_TYPES \
X(Window)\
X(Camera)\
X(States)\
X(Render)\
X(GlyphDecoder)\
X(Audio)\
X(Listener)\
X(Cursor)\
X(KeyButtons)\
X(MouseButtons)\
X(MouseScroll)

namespace mtrs::comp
{

ECSWorld::ECSWorld(std::string scenes_dir, std::unordered_set<std::string> paths,
    uint64_t limit_size_cache)
: _file_manager(paths, limit_size_cache), _scenes_dir(std::move(scenes_dir))
{
    _templates.reserve(paths.size());
    std::string name;
    for(auto &path : paths)
    {
        name = path.substr(_scenes_dir.length(), (path.length() - _scenes_dir.length() - 6));
        _templates.emplace(math::hash64(name), name);
    }
}

ECSWorld::ECSWorld(std::string scenes_dir, uint64_t limit_size_cache)
: ECSWorld(scenes_dir, fs::get_files_from_folder(scenes_dir, ".mtscn"), limit_size_cache)
{
}

ECSWorld::ECSWorld(ECSWorld &&other) noexcept
: _file_manager(std::move(other._file_manager))
{
    _templates = std::move(other._templates);
    _scenes = std::move(other._scenes);
    _pending_ops = std::move(other._pending_ops);
    _scenes_dir = std::move(other._scenes_dir);
    _resources = other._resources;
    _components = std::move(other._components);
    _destroy_ids = std::move(other._destroy_ids);
    _freed_ids = std::move(other._freed_ids);
}

ECSWorld &ECSWorld::operator=(ECSWorld &&other) noexcept
{
    if(this != &other)
    {
        _templates = std::move(other._templates);
        _scenes = std::move(other._scenes);
        _pending_ops = std::move(other._pending_ops);
        _scenes_dir = std::move(other._scenes_dir);
        _resources = other._resources;
        _components = std::move(other._components);
        _file_manager = std::move(other._file_manager);
        _destroy_ids = std::move(other._destroy_ids);
        _freed_ids = std::move(other._freed_ids);
    }
    return *this;
}

ECSWorld::~ECSWorld()
{
    
}

void ECSWorld::set_resources(res::ResourceManager *resources)
{
    _resources = resources;
}

prs::MtrsFileManager::MtrsFile *ECSWorld::open(uint64_t tmp_hash)
{
    auto tmp_iter = _templates.find(tmp_hash);
#ifndef FLAG_RELEASE
    if(tmp_iter == _templates.end())
    {
        msg::mtrs_error("Failed to open scene, scene template naming \"",
            math::rehash64(tmp_hash),"\" does not exist");
        return nullptr;
    }
#endif

    auto file = _file_manager.get_file(_scenes_dir + tmp_iter->second + ".mtscn");

#ifndef FLAG_RELEASE
    std::string_view magic(file ? file->data.data() : "", 7);
    if(!file || magic != "mtrsscn")
    {
        msg::mtrs_error("Failed to open scene,",
            " scene template naming \"", math::rehash64(tmp_hash),
            "\" does not have the magic mtrsscn ", magic);
        return nullptr;
    }
#endif

    return file;
}

void ECSWorld::load_scene(uint64_t tmp_hash, uint64_t scn_hash)
{
#ifndef FLAG_RELEASE
    if(!_templates.count(tmp_hash))
    {
        msg::mtrs_error("Failed to load scene, there is no template naming \"",
            math::rehash64(tmp_hash), '"');
        return;
    }
#endif

    _pending_ops.push_back({SceneOp::Load, tmp_hash, scn_hash});
}

void ECSWorld::do_load(uint64_t tmp_hash, uint64_t scn_hash)
{
    auto file = open(tmp_hash);
    if(!file) return;

    auto [scene_iter, inserted] = _scenes.try_emplace(scn_hash, Scene{tmp_hash, {}, true});
#ifndef FLAG_RELEASE
    if(!inserted)
    {
        msg::mtrs_warning("Failed to load scene, there is already an instance named \"",
            math::rehash64(scn_hash), '"');
        return;
    }
#endif

    char *data = file->data.data();
    size_t cur = 16;

    uint32_t entity_count = 0;
    uint32_t data_offset;
    uint32_t dynamic_date_offset;
    
    read(entity_count, data, cur);
    read(data_offset, data, cur);
    read(dynamic_date_offset, data, cur);

#ifndef FLAG_RELEASE
    if(cur != data_offset)
    {
        msg::mtrs_error("Failed to load scene,",
            " data_offset does not match the value ", data_offset);
        return;
    }
#endif

    for(int i = 0; i < entity_count; i++)
    {
        uint64_t id, offset;
        EntityID entity;
        read(id, data, cur);
        read(offset, data, cur);
        if(_freed_ids.empty())
        {
            entity = _components.create_entity();
        }
        else
        {
            entity = _freed_ids.top();
            _freed_ids.pop();
        }
        scene_iter->second.local_entities.emplace(id, entity);

        uint64_t id_comp;
        while(cur < offset)
        {
            read(id_comp, data, cur);
            switch (id_comp)
            {
#define X(Comp) case math::hash64_(#Comp): \
{ _components.add_comp<Comp>(entity, entity, scn_hash, tmp_hash, data + cur, file->deferred_data, *this, *_resources); \
cur += Comp::get_prs_size(); } break;
            COMPONENT_TYPES
#undef X
#ifndef FLAG_RELEASE
            default:
                msg::mtrs_error("Unknown component by id ", id_comp, ", in entity with id ", id);
                return;
#endif
            }
        }
    }
}

void ECSWorld::remove_scene(uint64_t scn_hash)
{
    _pending_ops.push_back({SceneOp::Remove, 0, scn_hash});
}

void ECSWorld::do_remove(uint64_t scn_hash)
{
    auto iter = _scenes.find(scn_hash);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        msg::mtrs_error("Failed to remove scene,",
            "there is no loaded scene named \"", math::rehash64(scn_hash), '"');
        return;
    }
#endif

    for(auto &entity : iter->second.local_entities)
    {
        mark_destroy(entity.second);
    }

    _scenes.erase(iter);
}

void ECSWorld::turn_on_scene(uint64_t scn_hash)
{
    _pending_ops.push_back({SceneOp::TurnOn, 0, scn_hash});
}

void ECSWorld::do_turn_on(uint64_t scn_hash)
{
    auto iter = _scenes.find(scn_hash);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        msg::mtrs_error("Failed to turn on scene,",
            " there is no loaded scene named \"", math::rehash64(scn_hash), '"');
        return;
    }
#endif
    for(auto &entity : iter->second.local_entities)
    {
        _components.turn_on(entity.second);
    }
    iter->second.turn_on = true;
}

void ECSWorld::turn_off_scene(uint64_t scn_hash)
{
    _pending_ops.push_back({SceneOp::TurnOff, 0, scn_hash});
}

void ECSWorld::do_turn_off(uint64_t scn_hash)
{
    auto iter = _scenes.find(scn_hash);
#ifndef FLAG_RELEASE
    if(iter == _scenes.end())
    {
        msg::mtrs_error("Failed to turn off scene,",
            " there is no loaded scene named \"", math::rehash64(scn_hash), '"');
        return;
    }
#endif
    _components.reserve_turn_off(iter->second.local_entities.size());
    for(auto &entity : iter->second.local_entities)
    {
        _components.turn_off(entity.second);
    }
    iter->second.turn_on = false;
}

void ECSWorld::mark_destroy(EntityID entity)
{
    _destroy_ids.push_back(entity);
}

void ECSWorld::update(const double &delta)
{
    for(size_t i = 0; i < _pending_ops.size(); i++)
    {
        auto &op = _pending_ops[i];
        switch(op.op)
        {
        case SceneOp::Load:
            do_load(op.tmp_hash, op.scn_hash);
            break;
        case SceneOp::Remove:
            do_remove(op.scn_hash);
            break;
        case SceneOp::TurnOn:
            do_turn_on(op.scn_hash);
            break;
        case SceneOp::TurnOff:
            do_turn_off(op.scn_hash);
            break;
        }
    }

    _pending_ops.clear();

    for(auto &entity : _destroy_ids)
    {
#define X(Comp) _components.remove_comp<Comp>(entity);
        COMPONENT_TYPES
#undef X
        _freed_ids.push(entity);
    }

    _destroy_ids.clear();

    _file_manager.update(delta);
}

void ECSWorld::clear_all()
{
#define X(Comp) _components.clear_set<Comp>();
    COMPONENT_TYPES
#undef X
#define X(Comp) _components.remove_single_comp<Comp>();
    SINGLE_COMPONENT_TYPES
#undef X

    _templates.clear();
    _scenes.clear();
    _pending_ops.clear();
    _resources = nullptr;

    _file_manager.clear();
}

void *ECSWorld::single_comp(uint64_t hash_comp)
{
    switch(hash_comp)
    {
#define X(Comp) case math::hash64_(#Comp): \
return _components.get_single_comp<Comp>();
        SINGLE_COMPONENT_TYPES
#undef X
    }
    return nullptr;
}

void *ECSWorld::component(uint64_t hash_comp, EntityID entity)
{
    switch(hash_comp)
    {
#define X(Comp) case math::hash64_(#Comp): \
return _components.get_comp<Comp>(entity);
        COMPONENT_TYPES
#undef X
    }
    return nullptr;
}

EntityID ECSWorld::get_entity(uint64_t scn_hash, uint64_t ent_hash)
{
    auto scene_iter = _scenes.find(scn_hash);
#ifndef FLAG_RELEASE
    if(scene_iter == _scenes.end())
    {
        msg::mtrs_error("Failed to find entity,",
            " there is no loaded scene named \"", math::rehash64(scn_hash), '"');
        return NULL_ENTITY;
    }
#endif

    auto entity_iter = scene_iter->second.local_entities.find(ent_hash);
#ifndef FLAG_RELEASE
    if(entity_iter == scene_iter->second.local_entities.end())
    {
        msg::mtrs_error("Failed to find entity,",
            " there is no entity with hash <", math::rehash64(ent_hash),
            "> in the scene named \"", math::rehash64(scn_hash), '"');
        return NULL_ENTITY;
    }
#endif

    return entity_iter->second;
}

bool ECSWorld::save_static_to_file(uint64_t scn_hash, uint64_t ent_hash,
    uint64_t hash_comp, size_t field, void *new_data, size_t size)
{
    auto scene_iter = _scenes.find(scn_hash);
    if(scene_iter == _scenes.end())
    {
#ifndef FLAG_RELEASE
        msg::mtrs_warning("Unable to save the static data field to file\n",
            "\tArgs: scene{", math::rehash64(scn_hash), "}, entity{", math::rehash64(ent_hash), "}");
#endif
        return false;
    }

    auto file = open(scene_iter->second.tmp_hash);
    if(!file) return false;

    char *data = file->data.data();
    size_t cur = 16;

    uint32_t entity_count = 0;
    uint32_t data_offset;
    uint32_t dynamic_offset;

    read(entity_count, data, cur);
    read(data_offset, data, cur);
    read(dynamic_offset, data, cur);

    for(int i = 0; i < entity_count && cur < dynamic_offset; i++)
    {
        uint64_t id, offset;
        read(id, data, cur);
        read(offset, data, cur);

        if(id != ent_hash)
        {
            cur = offset;
        }
        else
        {
            while(cur < offset)
            {
                uint64_t id_comp;
                read(id_comp, data, cur);

                if(id_comp == hash_comp)
                {
                    cur += field;
                    std::memcpy(data + cur, new_data, size);
                    file->dirty = true;
                    return true;
                }

                switch(id_comp)
                {
#define X(Comp) case math::hash64_(#Comp): cur += Comp::get_prs_size(); break;
                    COMPONENT_TYPES
#undef X
                default:
#ifndef FLAG_RELEASE
                    msg::mtrs_warning("Unable to save the static data field to file\n",
                        "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{",
                        math::rehash64(ent_hash), "}, unknown component id ", id_comp);
#endif
                    return false;
                }
            }
            break;
        }
    }

#ifndef FLAG_RELEASE
    msg::mtrs_warning("Unable to find and save the static data field to file\n",
        "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{", math::rehash64(ent_hash), "}");
#endif

    return false;
}

bool ECSWorld::save_dynamic_to_file(uint64_t scn_hash, uint64_t ent_hash,
    uint64_t hash_comp, size_t field, void *new_data, size_t size)
{
    auto scene_iter = _scenes.find(scn_hash);
    if(scene_iter == _scenes.end())
    {
#ifndef FLAG_RELEASE
        msg::mtrs_warning("Unable to save the deferred data field to file\n",
            "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{", math::rehash64(ent_hash), "}");
#endif
        return false;
    }

    auto file = open(scene_iter->second.tmp_hash);
    if(!file) return false;

    char *data = file->data.data();
    size_t cur = 16;

    uint32_t entity_count = 0;
    uint32_t data_offset;
    uint32_t dynamic_offset;

    read(entity_count, data, cur);
    read(data_offset, data, cur);
    read(dynamic_offset, data, cur);

    for(int i = 0; i < entity_count && cur < dynamic_offset; i++)
    {
        uint64_t id, offset;
        read(id, data, cur);
        read(offset, data, cur);

        if(id != ent_hash)
        {
            cur = offset;
        }
        else
        {
            while(cur < offset)
            {
                uint64_t id_comp;
                read(id_comp, data, cur);

                if(id_comp != hash_comp)
                {
                    switch(id_comp)
                    {
#define X(Comp) case math::hash64_(#Comp): cur += Comp::get_prs_size(); break;
                        COMPONENT_TYPES
#undef X
                    default:
#ifndef FLAG_RELEASE
msg::mtrs_warning("Unable to save the deferred data field to file\n",
                        "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{",
                        math::rehash64(ent_hash), "}, unknown component id ", id_comp);
#endif
                        return false;
                    }
                }
                else
                {
                    cur += field;
                    uint64_t *field_ptr = reinterpret_cast<uint64_t*>(data + cur);
                    auto iter = file->deferred_data.find(*field_ptr);
                    if(iter == file->deferred_data.end())
                    {
#ifndef FLAG_RELEASE
                        msg::mtrs_warning("Unable to find the deferred data field to save in file\n",
                            "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{",
                            math::rehash64(ent_hash), "}");
#endif
                        return false;
                    }

                    prs::DeferredData &ddata = iter->second;

                    delete[] ddata.data;
                    ddata.data = new char[size];
                    std::memcpy(ddata.data, new_data, size);

                    file->data_size += size - ddata.field[1];
                    ddata.field[1] = size;

                    file->dirty = true;
                    return true;
                }
            }
            break;
        }
    }

#ifndef FLAG_RELEASE
    msg::mtrs_warning("Unable to find and save the deferred data field to file\n",
        "\tArgs: scene{", math::rehash64(scn_hash), "}, ent_hash{", math::rehash64(ent_hash), "}");
#endif

    return false;
}

}