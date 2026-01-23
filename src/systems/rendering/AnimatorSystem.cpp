#include "systems/rendering/AnimatorSystem.h"

#include "components/rendering/Sprite.h"
#include "components/rendering/Animator.h"
#include "ECSWorld.h"

#include <iostream>

void AnimatorSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, sprite, animator] : world.view<Sprite2D, Animator>())
    {
        animator->currentAnimationTime += delta;

        while (animator->currentAnimationTime >= animator->durations[animator->currentFrame])
        {
            animator->currentAnimationTime -= animator->durations[animator->currentFrame];
            animator->currentFrame++;

            if (animator->currentFrame == animator->count + animator->offset)
            {
                animator->currentFrame = animator->offset;
            }

            sprite->subTexture = sprite->atlas->getSubTexture(animator->currentFrame);
        }
    }
}