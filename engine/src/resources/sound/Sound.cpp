#include "Sound.h"


#include "../../external/miniaudio.h"

#include <string>

#ifndef FLAG_RELEASE
#include <iostream>
#endif

Sound::Sound(ma_engine *engine, const char *path, ma_uint32 flag)
: _thisSound(new ma_sound)
{
    ma_result result = ma_sound_init_from_file(engine, path, flag, nullptr, nullptr, _thisSound);
    if(result != MA_SUCCESS)
    {
#ifndef FLAG_RELEASE
        std::cerr << "Failed init sould to path " << path << " " << result << std::endl;
#endif
    }
}

Sound::~Sound()
{
    ma_sound_uninit(_thisSound);
}

void Sound::play()
{
    ma_sound_start(_thisSound);
}

void Sound::stop()
{
    ma_sound_stop(_thisSound);
}

void Sound::set_volume(float volume)
{
    ma_sound_set_volume(_thisSound, volume);
}

void Sound::set_position(float x, float y, float z)
{
    ma_sound_set_position(_thisSound, x, y, z);
}

bool Sound::is_playing() const
{
    return ma_sound_is_playing(_thisSound);
}