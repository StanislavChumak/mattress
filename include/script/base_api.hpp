#ifndef BASE_API_HPP
#define BASE_API_HPP

#include "engine_api.hpp"
#include "res/asset/ScriptFile.hpp"
#include "comp/core/ScriptUpdate.hpp"
#include "comp/core/ScriptCallback.hpp"
#include "comp/core/StoredData.hpp"
#include "comp/core/Transform.hpp"

typedef mtrs::comp::ScriptUpdate ScriptUpdate;
typedef mtrs::comp::ScriptCallback ScriptCallback;
typedef mtrs::comp::StoredData StoredData;
typedef mtrs::comp::Transform Transform;

extern mtrs::EngineAPI *api;

namespace mtrs::res
{
    void *ScriptFile::get_symbol(const char *name)
    {
        return api->script_get_symbol(this, name);
    }
}

template<typename Fun>
Fun ScriptCallback::get_fun_script(const char *name)
{
    return reinterpret_cast<Fun>(script_file->get_symbol(name));
}


#endif
