#include "sys/audio/LocationAudioSystem.h"

#include "comp/core/Transform.h"

#include "comp/single/Listener.h"
#include "comp/audio/Sound.h"
#include "comp/audio/Music.h"

void LocationAudioSystem::update(ECSWorld &world, const double &delta)
{
    Listener *listener = world.get_single_comp<Listener>();
    Audio *audio = world.get_single_comp<Audio>();
    if(!listener || !audio || (listener->target != NULL_ENTITY)) return;
    glm::vec2 listenerPosition = world.get_comp<Transform>(listener->target)->position / audio->soundScale;
    ma_engine_listener_set_position(&audio->engine, 0, listenerPosition.x, listenerPosition.y, 0.f);
    for(auto [entity, sound, transform] : world.view<Sound, Transform>())
    {
        if(sound->isLocation)
        {
            for(auto ptrSound : sound->instances)
                ma_sound_set_position(ptrSound, transform->globalMatrix[3].x / audio->soundScale
                                              , transform->globalMatrix[3].y / audio->soundScale
                                              , transform->globalMatrix[3].z / audio->soundScale);

        }
    }
    for(auto [entity, music, transform] : world.view<Music, Transform>())
    {
        if(music->isLocation)
            ma_sound_set_position(music->music, transform->globalMatrix[3].x * audio->soundScale
                                              , transform->globalMatrix[3].y * audio->soundScale
                                              , transform->globalMatrix[3].z * audio->soundScale);
    }
}