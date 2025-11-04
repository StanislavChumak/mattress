#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#define MINIAUDIO_IMPLEMENTATION
#include "../../../external/miniaudio.h"

struct Audio
{
    ma_engine engine;
    bool initialized = false;

    void init()
    {
        if (ma_engine_init(nullptr, &engine) == MA_SUCCESS)
            initialized = true;
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
