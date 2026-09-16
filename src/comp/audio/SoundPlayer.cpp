#include "comp/audio/SoundPlayer.hpp"

#include "comp/ECSWorld.hpp"
#include "comp/single/Audio.hpp"
#include "res/ResourceManager.hpp"
#include "res/asset/Sound.hpp"

#include "util/fun/prs/mtrs_file.hpp"
#include "util/fun/math/hash.hpp"
#include "util/type/prs/comp/SoundPlayer.hpp"

#include <cstring>

namespace mtrs::comp
{

SoundPlayer::SoundPlayer(COMPONENT_ARGS)
{
    prs::SoundPlayer sound_player;
    std::memcpy(&sound_player, file_data, sizeof(sound_player));

    std::string sound_path;
    prs::set_mtrs_to_var(file_ddata[sound_player.sound], sound_path);
    sound = resource.get_resource<res::Sound>(tmp_hash, sound_path);

    sound->init(static_cast<Audio*>(world.single_comp(math::hash64(Audio::get_type_name())))->impl);

    sound->set_volume(sound_player.volume);
}

uint32_t SoundPlayer::get_prs_size_imp() noexcept
{
    return sizeof(prs::SoundPlayer);
}


}