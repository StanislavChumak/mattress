#include "comp/core/ScriptUpdate.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/ScriptUpdate.hpp"

#include <cstring>

namespace mtrs::comp
{

ScriptUpdate::ScriptUpdate(COMPONENT_ARGS)
{
    prs::ScriptUpdate script;
    std::memcpy(&script, file_data, sizeof(script));

    std::string path;
    prs::set_mtrs_to_var(file_ddata[script.script_file], path);

    script_file = resource.get_resource<res::ScriptFile>(tmp_hash, path);
    update = reinterpret_cast<decltype(update)>(script_file->get_symbol("update"));
}

uint32_t ScriptUpdate::get_prs_size_imp() noexcept
{
    return sizeof(prs::ScriptUpdate);
}

}