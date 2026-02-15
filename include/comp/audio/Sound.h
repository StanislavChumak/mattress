#ifndef SOUND_H
#define SOUND_H

#include "comp/single/Audio.h"

#include "jsonUtils.h"

#include "comp/ECSWorld.h"

class ResourceManager;

struct Sound
{
    std::vector<ma_sound*> instances;
    size_t count;

    float scaleVolume = 1.f;
    bool isLocation = false;

    bool isPlay = false;
    bool isStop = false;


    Sound() = default;
    Sound(const Sound&) = delete;
    Sound &operator=(const Sound&) = delete;
    Sound(Sound &&other)
    {
        instances = other.instances;
        count = other.count;
        scaleVolume = other.scaleVolume;
        isLocation = other.isLocation;

        other.instances.clear();
    }
    Sound &operator=(Sound &&other)
    {
        if(this == &other) return *this;
        for(auto sound : instances) ma_sound_uninit(sound);
        instances = other.instances;
        count = other.count;
        scaleVolume = other.scaleVolume;
        isLocation = other.isLocation;

        other.instances.clear();
        return *this;
    }
    ~Sound()
    {
        for(auto sound : instances) ma_sound_uninit(sound);
    }

    void from_json(simdjson::ondemand::object obj, EntityID id, ECSWorld &world, ResourceManager &resource)
    {
        Audio *audio = world.get_single_comp<Audio>();
        if(!audio) return;

        std::string path = std::string(get_var_json<std::string_view>(obj["path"]));

        count = get_var_json<int64_t>(obj["count"]);
        instances.resize(count);

        auto resultDouble = obj["scaleVolume"].get_double();
        if(!resultDouble.error()) scaleVolume = resultDouble.value();

        auto resultBool = obj["location"].get_bool();
        if(!resultBool.error()) isLocation = resultBool.value();

        for(int i = 0; i < count; i++)
        {
            instances[i] = new ma_sound;
            ma_result resultSound = ma_sound_init_from_file(&audio->engine, path.c_str(), (isLocation ? 0 : MA_SOUND_FLAG_NO_SPATIALIZATION), nullptr, nullptr, instances[i]);
#ifndef FLAG_RELEASE
            if(resultSound != MA_SUCCESS)
            {
                std::cerr << "Failed init sould to path " << path << " " << resultSound << std::endl;
            }
#endif
        }

        if(isLocation)
        {
            auto resultInt = obj["maxDistance"].get_int64();
            if(!resultInt.error())
                for(auto sound : instances)
                    ma_sound_set_max_distance(sound, resultInt.value());
            else
                for(auto sound : instances)
                    ma_sound_set_max_distance(sound, audio->soundScale);
        }
    }

    void setVolume(float volume)
    {
        for(auto sound : instances)
            ma_sound_set_volume(sound, volume * scaleVolume);
    }

    ma_sound *getFreeSound()
    {
        for(auto sound : instances)
            if(!ma_sound_is_playing(sound)) return sound;
        return nullptr;
    }
};

#endif
