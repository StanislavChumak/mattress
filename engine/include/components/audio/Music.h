#ifndef MUSIC_H
#define MUSIC_H

#include "Audio.h"

#include "../external/jsonUtils.h"

class ResourceManager;

struct Music
{
    ma_sound *music;

    float scaleVolume = 1.f;

    bool isPlay = false;
    bool isStop = false;

    bool isLocation;

    Music() = default;
    Music& operator=(const Music&) = default;
    Music(Music &&music)
    {
        this->music = music.music;
        scaleVolume = music.scaleVolume;

        this->music = nullptr;
    }
    Music &operator=(Music &&music)
    {
        if(this->music)
            ma_sound_uninit(this->music);
        this->music = music.music;
        scaleVolume = music.scaleVolume;

        this->music = nullptr;
        return *this;
    }
    ~Music()
    {
        if(music)
            ma_sound_uninit(music);
    }

    virtual void fromJson(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        Audio *audio = world.getSingleComponent<Audio>();
        if(!audio) return;

        std::string path = std::string(getVarJSON<std::string_view>(obj["path"]));

        auto resultDouble = obj["scaleVolume"].get_double();
        if(!resultDouble.error()) scaleVolume = resultDouble.value();

        isLocation = getVarJSON<bool>(obj["location"]);

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
