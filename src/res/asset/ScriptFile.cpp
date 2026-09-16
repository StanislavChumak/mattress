#include "res/asset/ScriptFile.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Window.hpp"
#include "comp/single/Camera.hpp"
#include "comp/single/GlyphDecoder.hpp"
#include "comp/single/KeyButtons.hpp"
#include "comp/single/MouseButtons.hpp"
#include "comp/single/MouseScroll.hpp"
#include "comp/single/Cursor.hpp"

#include "res/asset/TextureAtlas.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/msg/mtrs_message.hpp"

#include "util/type/prs/res/ScriptFile.hpp"

#include <cstring>

namespace mtrs::res
{

ScriptFile::ScriptFile(RESOURCE_ARGS)
{
    prs::ScriptFile script_file;
    std::memcpy(&script_file, file_data, sizeof(script_file));

    std::string path;
    prs::set_mtrs_to_var(file_ddata[script_file.path], path);
    path = path.substr(0, path.rfind('.'));

    _handle = fs::load_library((dir_pack + path + fs::lib_extension()).c_str());
#ifndef FLAG_RELEASE
    if (_handle)
    {
#endif
        _on_load = reinterpret_cast<decltype(_on_load)>(fs::get_symbol(_handle, "on_load"));
        _on_unload = reinterpret_cast<decltype(_on_unload)>(fs::get_symbol(_handle, "on_unload"));
#ifndef FLAG_RELEASE
        if(!_on_load) msg::mtrs_error(fs::get_last_error());
        if(!_on_unload) msg::mtrs_error(fs::get_last_error());
    }
    else
    {
        msg::mtrs_error("Failed to load script at path: ", path, "\n", fs::get_last_error());
    }
#endif
}

ScriptFile::ScriptFile(ScriptFile &&other) noexcept
{
    _api = std::move(other._api);
    _handle = other._handle;
    other._handle = nullptr;
    _on_load = other._on_load;
    other._on_load = nullptr;
    _on_unload = other._on_unload;
    other._on_unload = nullptr;
}

ScriptFile &ScriptFile::operator=(ScriptFile &&other) noexcept
{
    if(this != &other)
    {
        _api = std::move(other._api);
        _handle = other._handle;
        other._handle = nullptr;
        _on_load = other._on_load;
        other._on_load = nullptr;
        _on_unload = other._on_unload;
        other._on_unload = nullptr;
    }
    return *this;
}

ScriptFile::~ScriptFile()
{
    if(_handle)
    {
        _on_unload();
        fs::free_library(_handle);
        _handle = nullptr;
    }

    if(_api.scene)
    {
        delete[] _api.scene;
    }
}

uint32_t ScriptFile::get_prs_size_imp() noexcept
{
    return sizeof(prs::ScriptFile);
}

void ScriptFile::load(const char *scene, comp::EntityID entity,
    comp::ECSWorld& world, ResourceManager& resource)
{
    // base
    _api.world = &world;
    _api.resource = &resource;
    char *copy = new char[strlen(scene)+1];
    strcpy(copy, scene);
    _api.scene = copy;

    // util
#ifndef FLAG_RELEASE
    _api.message = [](msg::TypeMessage tmsg, const char *msg)
        { msg::detail::show_message(tmsg, msg); };
#else
    _api.message = [](mtrs::util::TypeMessage tmsg, const char *msg) {};
#endif

    // ECSWorld
    _api.world_load_scene = [](comp::ECSWorld*w, res::ResourceManager*r, const char*s, bool is)
        { w->load_scene(s, *r, is); };
    _api.world_remove_scene = [](comp::ECSWorld*w, const char*s)
        { w->remove_scene(s); };
    _api.world_turn_on_scene = [](comp::ECSWorld*w, const char*s)
        { w->turn_on_scene(s); };
    _api.world_turn_off_scene = [](comp::ECSWorld*w, const char*s)
        { w->turn_off_scene(s); };
    _api.world_single_comp = [](comp::ECSWorld*w, uint64_t c)
        { return w->single_comp(c); };
    _api.world_component = [](comp::ECSWorld*w, uint64_t c, comp::EntityID e)
        { return w->component(c, e); };
    _api.world_get_entity = [](comp::ECSWorld*w, const char *s, uint64_t h)
        { return w->get_entity(s, h); };
    _api.world_save_static = [](comp::ECSWorld*w, const char*s, uint64_t e, uint64_t c, uint64_t f, void*d, uint64_t sz)
        { return w->save_static_to_file(s, e, c, f, d, sz); };
    _api.world_save_dynamic = [](comp::ECSWorld*w, const char*s, uint64_t e, uint64_t c, uint64_t f, void*d, uint64_t sz)
        { return w->save_dynamic_to_file(s, e, c, f, d, sz); };

    // window
    _api.window_set_icon = [](comp::Window*w, const char *const *p, uint64_t c){ w->set_icon(p, c); };
    _api.window_set_position = [](comp::Window*w, glm::uvec2 v){ w->set_position(v); };
    _api.window_set_full_screen = [](comp::Window*w, bool is){ w->set_full_screen(is); };
    _api.window_get_position = [](comp::Window*w){ return w->get_position(); };
    _api.window_subscribe_to_size = [](comp::Window*w, void (*c)()){ w->subscribe_to_size(c); };
    _api.window_unsubscribe_to_size = [](comp::Window*w, void (*c)()){ w->unsubscribe_to_size(c); };

    // camera
    _api.camera_update_UBO = [](comp::Camera*c){ c->update_UBO(); };
    _api.camera_update_proj_matrix = [](comp::Camera*c){ c->update_proj_matrix(); };
    _api.camera_update_view_matrix = [](comp::Camera*c){ c->update_view_matrix(); };

    // glyph decoder
    _api.decoder_submit_font = [](comp::GlyphDecoder*d, const char*p){ d->submit_font(p); };
    _api.decoder_decode_text = [](comp::GlyphDecoder*d, const char32_t*s){ return d->decode_text(s); };

    // key buttons
    _api.key_subscribe = [](comp::KeyButtons*kb, int k, bool a, void (*c)()){ kb->subscribe(k, a, c); };
    _api.key_unsubscribe = [](comp::KeyButtons *kb, int k, bool a, void (*c)()){ kb->unsubscribe(k, a, c); };

    // mouse buttons
    _api.mouse_subscribe = [](comp::MouseButtons *ms, int b, bool a, void (*c)()){ ms->subscribe(b, a, c); };
    _api.mouse_unsubscribe = [](comp::MouseButtons *ms, int b, bool a, void (*c)()){ ms->unsubscribe(b, a, c); };
    
    // mouse scroll
    _api.scroll_subscribe = [](comp::MouseScroll *sc, void (*c)()){ sc->subscribe(c); };
    _api.scroll_unsubscribe = [](comp::MouseScroll *sc, void (*c)()){ sc->unsubscribe(c); };
    
    // cursor
    _api.cursor_subscribe = [](comp::Cursor *crs, void (*c)()){ crs->subscribe(c); };
    _api.cursor_unsubscribe = [](comp::Cursor *crs, void (*c)()){ crs->unsubscribe(c); };

    
    // script file
    _api.script_get_symbol = [](res::ScriptFile *s, const char *n){ return s->get_symbol(n); };

    // texture atlas
    _api.atlas_get_sub_texture = [](const res::TextureAtlas *a, size_t i){ return a->get_sub_texture(i); };

    _on_load(entity, &_api);
}

void *ScriptFile::get_symbol(const char *name)
{
#ifndef FLAG_RELEASE
    if(!_handle)
    {
        msg::mtrs_error("Attempt to get a ", name, " from a script that is not loaded");
        return nullptr;
    }
    void *symbol = fs::get_symbol(_handle, name);
    if(!symbol)
    {
        msg::mtrs_error("Failed to obtain \"", name,"\" due to: ", fs::get_last_error());
    }
    return symbol;
#elif
    return fs::get_symbol(_handle, name);
#endif
    
}

}