#include "systems/audio/AudioSystem.h"

#include "ECSWorld.h"

#define MINIAUDIO_IMPLEMENTATION

#include "components/audio/Sound.h"
#include "components/audio/Music.h"

void AudioSystem::update(ECSWorld &world, const double &delta)
{
    Audio *audio = world.getSingleComponent<Audio>();
    if(!audio) return;
    for(auto [entity, sound] : world.view<Sound>())
    {
        if(sound->isPlay)
        {
            auto ptrSound = sound->getFreeSound();
            if(ptrSound)
                ma_sound_start(ptrSound);
            sound->isPlay = false;
        }

        if(sound->isStop)
        {
            for(auto ptrSound : sound->instances)
                ma_sound_stop(ptrSound);
            sound->isStop;
        }

        if(audio->cacheVolumeDirty)
            sound->setVolume(audio->soundVolume);
    }

    for(auto [entity, music] : world.view<Music>())
    {
        if(music->isPlay)
            ma_sound_start(music->music);

        if(music->isStop)
            ma_sound_stop(music->music);

        if(audio->cacheVolumeDirty)
            music->setVolume(audio->musicVolume);
    }
}