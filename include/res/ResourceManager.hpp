#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "util/type/prs/MtrsFileManager.hpp"
#include "util/fun/msg/mtrs_message.hpp"
#include "util/fun/math/hash.hpp"

#include <memory>
#include <vector>
#include <string>

namespace mtrs::res
{

class ResourceManager
{
private:
    prs::MtrsFileManager _file_manager;    

    std::string _pack_dir;

    std::unordered_map<uint64_t, std::string> _hash_packs;

    size_t get_pos_resource(char *data, size_t end, const std::string &res_name,
        const std::string &prs_name, uint32_t prs_size);

    template<typename Resource>
    struct CacheWrapper
    {
        std::unordered_map<std::string, std::weak_ptr<Resource>> map;
        std::vector<std::string> keys;
    };

    template<typename Resource>
    CacheWrapper<Resource> &get_cache()
    {
        static CacheWrapper<Resource> wrapper;
        return wrapper;
    }

    ResourceManager(std::string pack_dir, std::unordered_set<std::string> paths,
        uint64_t limit_size_cache);

public:
    ResourceManager(std::string pack_dir, uint64_t limit_size_cache);

    ResourceManager() = delete;
    ResourceManager(ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&other) noexcept;
    ResourceManager &operator=(ResourceManager &&other) noexcept;
    ~ResourceManager();

    void update(const double &delta);
    void clear_all();

    const std::string &get_pack_dir() const noexcept { return _pack_dir; }

    template<typename Resource>
    std::shared_ptr<Resource> get_resource(uint64_t pack_hash, std::string resource_name)
    {
        size_t str_pos = resource_name.find_last_of("\\/");
        std::string pack;
        if(str_pos != std::string::npos)
        {
            pack = resource_name.substr(0, str_pos);
            resource_name = resource_name.substr(str_pos + 1);
        }
        else
        {
            auto it = _hash_packs.find(pack_hash);
#ifndef FLAG_RELEASE
            if(it == _hash_packs.end())
            {
                msg::mtrs_error("There is no pack named \"", math::rehash64(pack_hash),"\" in the ResourceManager");
                return std::shared_ptr<Resource>(nullptr);
            }
            else
#endif
            {
                pack = it->second;
            }
        }
        if(pack.find(_pack_dir) == 0) pack = pack.substr(_pack_dir.length());
        if(pack == "null") return std::shared_ptr<Resource>(nullptr);
        auto &cache = get_cache<Resource>();
        std::string full_res_name = pack + '/' + resource_name;
        auto it_res = cache.map.find(full_res_name);
        if(it_res != cache.map.end())
        {
            auto existing = it_res->second.lock();
            if(existing)
            {
                return std::static_pointer_cast<Resource>(existing);
            }
        }
        else
        {
            cache.keys.push_back(full_res_name);
        }

        auto file = _file_manager.get_file(_pack_dir + pack + ".mtpck");
        if(file)
        {
            std::string prs_name = Resource::get_type_name();
            uint32_t prs_size = Resource::get_prs_size();

            size_t res_pos = get_pos_resource(file->data.data(), file->data_size,
                resource_name, prs_name, prs_size);
            if(res_pos == 0)
            {
#ifndef FLAG_RELEASE
                msg::mtrs_error("from resource pack (", pack, ")");
#endif
                return std::shared_ptr<Resource>(nullptr);
            }
            
            std::shared_ptr<Resource> resource;
            str_pos = pack.find_last_of("\\/") + 1;
            resource = std::make_shared<Resource>(_pack_dir + pack.substr(0, str_pos),
                pack.substr(str_pos), file->data.data() + res_pos, file->deferred_data, *this);

#ifndef FLAG_RELEASE
            if(!resource)
            {
                msg::mtrs_error("failed to initialize resource (", prs_name, ": ",
                    resource_name, ") from resource pack (", pack, ")");
                return std::shared_ptr<Resource>(nullptr);
            }
#endif
            cache.map.emplace(full_res_name, std::weak_ptr<Resource>(resource));
            return resource;
        }
#ifndef FLAG_RELEASE
        msg::mtrs_error("No package named \"", pack,"\" found");
#endif
        return std::shared_ptr<Resource>(nullptr);
    }
};

}

#endif
