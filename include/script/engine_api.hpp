#ifndef ENGINE_API_HPP
#define ENGINE_API_HPP

#ifdef SCRIPT
    #include "util/fun/msg/mtrs_message.hpp"
#else
    #include <cstdint>
#endif

#if defined(_WIN32)
    #include <windows.h>
    #define EXPORT __declspec(dllexport)
#else
    #include <dlfcn.h>
    #define EXPORT __attribute__((visibility("default")))
#endif

#include "glm/fwd.hpp"

namespace mtrs::comp
{
    class ECSWorld;
    typedef uint64_t EntityID;

    struct Window;
    struct Camera;
    struct GlyphDecoder;
    struct KeyButtons;
    struct MouseButtons;
    struct MouseScroll;
    struct Cursor;
};

namespace mtrs::res
{
    class ResourceManager;

    class TextureAtlas;
    class ScriptFile;

    struct SubTexture;
    struct Text;
};

namespace mtrs::msg
{
    enum class TypeMessage;
}

namespace mtrs
{
    struct EngineAPI
    {
        comp::ECSWorld *world;
        res::ResourceManager *resource;
        uint64_t scn_hash = 0;

        // util
        void (*message)(mtrs::msg::TypeMessage, const char *) = nullptr;

        // ECSWorld
        void (*world_load_scene)(comp::ECSWorld*, res::ResourceManager*, uint64_t, uint64_t) = nullptr;
        void (*world_remove_scene)(comp::ECSWorld*, uint64_t) = nullptr;
        void (*world_turn_on_scene)(comp::ECSWorld*, uint64_t) = nullptr;
        void (*world_turn_off_scene)(comp::ECSWorld*, uint64_t) = nullptr;
        void *(*world_single_comp)(comp::ECSWorld*, uint64_t) = nullptr;
        void *(*world_component)(comp::ECSWorld*, uint64_t, comp::EntityID) = nullptr;
        comp::EntityID (*world_get_entity)(comp::ECSWorld*, uint64_t, uint64_t) = nullptr;
        bool (*world_save_static)(comp::ECSWorld*, uint64_t, uint64_t, uint64_t, uint64_t, void*, uint64_t) = nullptr;
        bool (*world_save_dynamic)(comp::ECSWorld*, uint64_t, uint64_t, uint64_t, uint64_t, void*, uint64_t) = nullptr;

        // Window
        void (*window_set_icon)(comp::Window*, const char *const *, uint64_t) = nullptr;
        void (*window_set_position)(comp::Window*, glm::uvec2) = nullptr;
        void (*window_set_full_screen)(comp::Window*, bool) = nullptr;
        glm::ivec2 (*window_get_position)(comp::Window*) = nullptr;
        void (*window_subscribe_to_size)(comp::Window*, void(*)()) = nullptr;
        void (*window_unsubscribe_to_size)(comp::Window*, void(*)()) = nullptr;

        // Camera
        void (*camera_update_UBO)(comp::Camera*) = nullptr;
        void (*camera_update_proj_matrix)(comp::Camera*) = nullptr;
        void (*camera_update_view_matrix)(comp::Camera*) = nullptr;

        // GlyphDecoder
        void (*decoder_submit_font)(comp::GlyphDecoder *, const char*);
        res::Text (*decoder_decode_text)(comp::GlyphDecoder *, const char32_t*);

        // KeyButtons
        void (*key_subscribe)(comp::KeyButtons*, int, bool, void(*)());
        void (*key_unsubscribe)(comp::KeyButtons*, int, bool, void(*)());

        // MouseButtons
        void (*mouse_subscribe)(comp::MouseButtons*, int, bool, void(*)());
        void (*mouse_unsubscribe)(comp::MouseButtons*, int, bool, void(*)());

        // MouseScroll
        void (*scroll_subscribe)(comp::MouseScroll*, void(*)());
        void (*scroll_unsubscribe)(comp::MouseScroll*, void(*)());

        // Cursor
        void (*cursor_subscribe)(comp::Cursor*, void(*)());
        void (*cursor_unsubscribe)(comp::Cursor*, void(*)());

        // ScriptFile
        void *(*script_get_symbol)(res::ScriptFile*, const char*) = nullptr;

        // TextureAtlas
        res::SubTexture (*atlas_get_sub_texture)(const res::TextureAtlas*, size_t) = nullptr;
    };
};

#ifdef SCRIPT

static mtrs::EngineAPI *api;

typedef mtrs::comp::EntityID EntityID;

#include "util/fun/math/hash.hpp"

namespace mtrs
{
    void load_scene(uint64_t template_hash, uint64_t instance_hash)
    {
        api->world_load_scene(api->world, api->resource, template_hash, instance_hash);
    }

    void remove_scene(uint64_t instance_hash)
    {
        api->world_remove_scene(api->world, instance_hash);
    }

    void turn_on_scene(uint64_t instance_hash)
    {
        api->world_turn_on_scene(api->world, instance_hash);
    }

    void turn_off_scene(uint64_t instance_hash)
    {
        api->world_turn_off_scene(api->world, instance_hash);
    }

    template<typename Component, uint64_t Hash = math::hash64_(Component::get_type_name())>
    Component *get_single_comp()
    {
        return reinterpret_cast<Component*>(api->world_single_comp(api->world, Hash));
    }

    template<typename Component, uint64_t Hash = math::hash64_(Component::get_type_name())>
    Component *get_comp(comp::EntityID entity)
    {
        return reinterpret_cast<Component*>(api->world_component(api->world, Hash, entity));
    }

    comp::EntityID get_entity(uint64_t scn_hash, uint64_t ent_hash)
    {
        return api->world_get_entity(api->world, scn_hash, ent_hash);
    }

    template<typename Component, typename T, uint64_t Hash = math::hash64_(Component::get_type_name())>
    bool save_static_to_file(uint64_t scn_hash, uint64_t ent_hash,
        uint64_t field_offset, T value)
    {
        T *data = new T{value};
        return api->world_save_static(api->world, scn_hash, ent_hash, Hash,
            field_offset, data, sizeof(T));
        delete data;
    }

    template<typename Component, uint64_t Hash = math::hash64_(Component::get_type_name())>
    bool save_dynamic_to_file(uint64_t scn_hash, uint64_t ent_hash,
        size_t field_offset, void *data, size_t size)
    {
        return api->world_save_dynamic(api->world, scn_hash, ent_hash, Hash,
            field_offset, data, size);
    }
}


namespace mtrs::msg::detail
{

void show_message(TypeMessage tmsg, std::string&& message)
{
    api->message(tmsg, message.c_str());
}

}

#endif

#endif
