#include "ECSWorld.h"

#include "components/ui/Cursor.h"
#include "components/audio/Sound.h"
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
    for(auto [entity, sound] : world.view<Sound>())
    {
        if(input->mouseButtons[0] && !input->lastMouseButtons[0])
        {
            sound->isPlay = true;
        }
    }
}