#include "systems/audio/LocationAudioSystem.h"

#include "components/core/Transform.h"

#include "components/audio/Listener.h"
#include "components/audio/Sound.h"
#include "components/audio/Music.h"

void LocationAudioSystem::update(ECSWorld &world, const double &delta)
{
    Listener *listener = world.getSingleComponent<Listener>();
    Audio *audio = world.getSingleComponent<Audio>();
    if(!listener && !audio) return;
    glm::vec2 listenerPosition = world.getComponent<Transform>(listener->target)->position / audio->soundScale;
    ma_engine_listener_set_position(&audio->engine, 0, listenerPosition.x, listenerPosition.y, 0.f);
    for(auto [entity, sound, transform] : world.view<Sound, Transform>())
    {
        if(sound->isLocation)
        {
            for(auto ptrSound : sound->instances)
                ma_sound_set_position(ptrSound, transform->worldMatrix[3].x / audio->soundScale
                                              , transform->worldMatrix[3].y / audio->soundScale
                                              , transform->worldMatrix[3].z / audio->soundScale);

        }
    }
    for(auto [entity, music, transform] : world.view<Music, Transform>())
    {
        if(music->isLocation)
            ma_sound_set_position(music->music, transform->worldMatrix[3].x * audio->soundScale
                                              , transform->worldMatrix[3].y * audio->soundScale
                                              , transform->worldMatrix[3].z * audio->soundScale);
    }
}