#ifndef MUSIC_H
#define MUSIC_H

#include "comp/single/Audio.h"

#include "jsonUtils.h"

class ResourceManager;

struct Music
{
    ma_sound *music;

    float scaleVolume = 1.f;
    bool isLocation = false;

    bool isPlay = false;
    bool isStop = false;


    Music() = default;
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;
    Music(Music &&other)
    {
        music = other.music;
        scaleVolume = other.scaleVolume;
        isLocation = other.isLocation;

        other.music = nullptr;
    }
    Music &operator=(Music &&other)
    {
        if(this == &other) return *this;
        if(music) ma_sound_uninit(music);
        music = other.music;
        scaleVolume = other.scaleVolume;
        isLocation = other.isLocation;

        other.music = nullptr;
        return *this;
    }
    ~Music()
    {
        if(music) ma_sound_uninit(music);
    }

    virtual void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        Audio *audio = world.get_single_comp<Audio>();
        if(!audio) return;

        std::string path = std::string(get_var_json<std::string_view>(obj["path"]));

        auto resultDouble = obj["scaleVolume"].get_double();
        if(!resultDouble.error()) scaleVolume = resultDouble.value();

        auto resultBool = obj["location"].get_bool();
        if(!resultBool.error()) isLocation = resultDouble.value();

        ma_result resultSound = ma_sound_init_from_file(&audio->engine, path.c_str(), (isLocation ? 0 : MA_SOUND_FLAG_NO_SPATIALIZATION) | MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_LOOPING, nullptr, nullptr, music);
#ifndef FLAG_RELEASE
        if(resultSound != MA_SUCCESS)
        {
            std::cerr << "Failed init music to path " << path << " " << resultSound << std::endl;
        }
#endif
    }

    void setVolume(float volume)
    {
        ma_sound_set_volume(music, volume * scaleVolume);
    }
};


#endif
