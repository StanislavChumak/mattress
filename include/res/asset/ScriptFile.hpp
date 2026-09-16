#ifndef SCRIPT_FILE_HPP
#define SCRIPT_FILE_HPP

#include "res/Resource.hpp"

#include "util/fun/fs/load_external_lib.hpp"
#include "script/engine_api.hpp"

namespace mtrs::res
{
class ResourceManager;

class ScriptFile : public Resource<ScriptFile>
{
    EngineAPI _api;
    fs::LibHandle _handle = nullptr;
    void (*_on_load)(mtrs::comp::EntityID, EngineAPI*) = nullptr;
    void (*_on_unload)() = nullptr;

public:
    ScriptFile(RESOURCE_ARGS);
    ScriptFile() = delete;
    ScriptFile(const ScriptFile&) = delete;
    ScriptFile &operator=(const ScriptFile&) = delete;
    ScriptFile(ScriptFile &&other) noexcept;
    ScriptFile &operator=(ScriptFile &&other) noexcept;
    ~ScriptFile();

    static constexpr const char *get_type_name_imp() noexcept { return "ScriptFile"; }
    static uint32_t get_prs_size_imp() noexcept;

    void load(uint64_t scn_hash, comp::EntityID entity,
        comp::ECSWorld &world, ResourceManager &resource);
    void *get_symbol(const char *name);
};

}

#endif
