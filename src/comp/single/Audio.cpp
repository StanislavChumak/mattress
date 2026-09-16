#include "comp/single/Audio.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace mtrs::comp
{

struct Impl
{
    ma_engine engine;
};

Audio::Audio()
{
    ma_engine_config config = ma_engine_config_init();
    config.listenerCount = 1;
    impl = new Impl();
    if (ma_engine_init(&config, &impl->engine) == MA_SUCCESS)
    {
        is_init = true;
    }
}

Audio::~Audio()
{
    if (is_init)
    {
        ma_engine_stop(&impl->engine);
        ma_engine_uninit(&impl->engine);
        is_init = false;
    }
    delete impl;
}

}