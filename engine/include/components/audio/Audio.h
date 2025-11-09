#ifndef AUDIO_H
#define AUDIO_H

#include "../external/miniaudio.h"

struct Audio
{
    ma_engine engine;
    bool initialized = false;

    float soundScale = 100.f;


    float soundVolume = 1.f;
    float musicVolume = 1.f;
    bool cacheVolumeDirty = false;

    void init()
    {
        ma_engine_config config = ma_engine_config_init();
        config.listenerCount = 1;
        if (ma_engine_init(&config, &engine) == MA_SUCCESS)
            initialized = true;
        ma_engine_listener_set_direction(&engine, 0, 0, 0, 1.f);
    }

    void destroy()
    {
        if (initialized)
        {
            ma_engine_stop(&engine);
            ma_engine_uninit(&engine);
            initialized = false;
        }
    }

    ~Audio()
    {
        destroy();
    }
};

#endif
