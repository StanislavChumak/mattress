#include "res/ResourceManager.hpp"

#include "util/fun/fs/get_folder.hpp"
#include "util/fun/math/hash.hpp"
#include "util/fun/prs/read.hpp"

#include "util/type/prs/res/res_types.hpp"
#include "res/asset/ScriptFile.hpp"
#include "res/asset/ShaderProgram.hpp"
#include "res/asset/Texture.hpp"
#include "res/asset/TextureAtlas.hpp"
#include "res/asset/Sound.hpp"
#include "res/asset/Font.hpp"

#include <algorithm>

namespace mtrs::res
{

ResourceManager::ResourceManager(std::string pack_dir, std::unordered_set<std::string> paths,
    uint64_t limit_size_cache)
: _file_manager(paths, limit_size_cache)
, _pack_dir(std::move(pack_dir))
{
    std::string name;
    for(auto &path : paths)
    {
        name = path.substr(_pack_dir.length(), (path.length() - _pack_dir.length() - 6));
        _hash_packs.emplace(math::hash64(name), name);
    }
}

ResourceManager::ResourceManager(std::string pack_dir, uint64_t limit_size_cache)
: ResourceManager(pack_dir, fs::get_files_from_folder(pack_dir, ".mtpck"), limit_size_cache)
{
}

ResourceManager::ResourceManager(ResourceManager &&other) noexcept
: _file_manager(std::move(other._file_manager))
{
    _pack_dir = std::move(other._pack_dir);
}

ResourceManager &ResourceManager::operator=(ResourceManager &&other) noexcept
{
    if(this != &other)
    {
        _file_manager = std::move(other._file_manager);
        _pack_dir = std::move(other._pack_dir);
    }
    return *this;
}

ResourceManager::~ResourceManager()
{

}

size_t ResourceManager::get_pos_resource(char *data, size_t end, const std::string &res_name,
        const std::string &prs_name, uint32_t prs_size)
{
    uint64_t res_type_id = math::hash64(prs_name);
    uint64_t res_id = math::hash64(res_name);

    size_t cur = 8;

    uint64_t id = 0, group_end = 0;
    while(cur < end)
    {
        read(id, data, cur);
        read(group_end, data, cur);
        if(id == res_type_id)
        {
            while(cur < group_end && cur < end)
            {
                read(id, data, cur);
                if(id == res_id)
                {
                    return cur;
                }
                cur += prs_size;
            }
            break;
        }
        cur = group_end;
    }
#ifndef FLAG_RELEASE
    msg::mtrs_error("Failed to find resource (",prs_name, ": ", res_name,")");
#endif
    return 0;
}

void ResourceManager::update(const double &delta)
{
#define X(Res)\
{\
    auto &cache = get_cache<Res>();\
    for(auto it_keys = cache.keys.begin(); it_keys != cache.keys.end();)\
    {\
        auto it_map = cache.map.find(*it_keys);\
        if(it_map->second.expired())\
        {\
           cache.map.erase(it_map);\
           std::iter_swap(it_keys, cache.keys.end() - 1);\
           cache.keys.pop_back();\
        }\
        else\
        {\
            it_keys++;\
        }\
    }\
}
    RESOURCE_TYPES
#undef X

    _file_manager.update(delta);
}

void ResourceManager::clear_all()
{
#define X(Res)\
{\
    auto &cache = get_cache<Res>();\
    cache.map.clear();\
    cache.keys.clear();\
}
    RESOURCE_TYPES
#undef X

    _file_manager.clear();
}

}
