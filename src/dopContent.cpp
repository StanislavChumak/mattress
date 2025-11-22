#include "ECSWorld.h"

#include "components/ui/Cursor.h"
#include "components/audio/Sound.h"
#include "components/core/Transform.h"
#include "components/core/Input.h"

class SoundCursorSystem
{
public:
    static void update(ECSWorld &world, const double &delta);
};

void SoundCursorSystem::update(ECSWorld &world, const double &delta)
{
    Input *input = world.getSingleComponent<Input>();
    if(!input) return;
    for(auto [entity, sound, transform] : world.view<Sound, Transform>())
    {
        if(input->mouseButtons[0] && !input->lastMouseButtons[0])
        {
            sound->isPlay = true;
            transform->rotation += 20.f;
        }
    }
}