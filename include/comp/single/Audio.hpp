#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "comp/Component.hpp"

namespace mtrs::comp
{

struct Audio : public Component<Audio>
{
    struct Impl *impl;

    bool is_init = false;

    float sound_scale = 100.f;

    float sound_volume = 1.f;
    float music_volume = 1.f;
    bool cache_volume_dirty = false;

    Audio(const Audio &) = delete;
    Audio &operator=(const Audio&) = delete;
    Audio(Audio&&) = delete;
    Audio &operator=(Audio&&) = delete;
    
    Audio();
    ~Audio();

    static constexpr const char *get_type_name_imp() noexcept { return "Audio"; }
};

}

#endif
