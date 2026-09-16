#include "comp/core/ScriptCallback.hpp"

#include "res/ResourceManager.hpp"
#include "res/asset/ScriptFile.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/type/prs/comp/ScriptCallback.hpp"

#include <cstring>

namespace mtrs::comp
{

ScriptCallback::ScriptCallback(COMPONENT_ARGS)
{
    prs::ScriptCallback script;
    std::memcpy(&script, file_data, sizeof(script));

    std::string path;
    prs::set_mtrs_to_var(file_ddata[script.script_file], path);

    script_file = resource.get_resource<res::ScriptFile>(tmp_hash, path);
    script_file->load(scn_hash, entity, world, resource);
}

uint32_t ScriptCallback::get_prs_size_imp() noexcept
{
    return sizeof(prs::ScriptCallback);
}

}