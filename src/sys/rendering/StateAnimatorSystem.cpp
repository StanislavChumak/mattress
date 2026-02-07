#include "sys/rendering/StateAnimatorSystem.h"

#include "comp/ECSWorld.h"
#include "comp/rendering/Animator.h"
#include "comp/rendering/StateAnimator.h"

void StateAnimatorSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, animator, state] : world.view<Animator, StateAnimator>())
    {
        if(!state->dirty) continue;

        auto it = state->states.find(state->currentState);
        if(it != state->states.end())
        {
            animator->currentFrame = animator->offset = it->second.first;
            animator->count = it->second.second;
            animator->currentAnimationTime = 0;
        }
        state->dirty = false;
    }
}