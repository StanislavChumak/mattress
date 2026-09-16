#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <unordered_map>
#include <cstdint>

namespace mtrs::res
{
    class ResourceManager;
}
    
namespace mtrs::comp
{
    typedef uint64_t EntityID;
    class ECSWorld;
}

namespace mtrs::prs
{
    struct DeferredData;
}

template<typename Derived>
class Component
{
public:
    static constexpr const char *get_type_name() noexcept
    {
        return Derived::get_type_name_imp();
    }

    static uint32_t get_prs_size() noexcept
    {
        return Derived::get_prs_size_imp();
    }
};

#define COMPONENT_ARGS mtrs::comp::EntityID entity, uint64_t scn_hash, uint64_t tmp_hash, \
    char *file_data, std::unordered_map<uint64_t, mtrs::prs::DeferredData> &file_ddata, \
    mtrs::comp::ECSWorld& world, mtrs::res::ResourceManager& resource   

#endif
