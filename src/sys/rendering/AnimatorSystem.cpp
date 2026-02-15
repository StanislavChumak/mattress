#include "sys/rendering/AnimatorSystem.h"

#include "comp/ECSWorld.h"
#include "comp/rendering/Sprite.h"
#include "comp/rendering/Animator.h"

#include <iostream>

void AnimatorSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, sprite, animator] : world.view<Sprite, Animator>())
    {
        animator->currentAnimationTime += delta;

        while (animator->currentAnimationTime >= animator->durations[animator->currentFrame])
        {
            animator->currentAnimationTime -= animator->durations[animator->currentFrame];
            animator->currentFrame++;

            if (animator->currentFrame == animator->countFrame + animator->offset)
            {
                animator->currentFrame = animator->offset;
            }

            sprite->subTexture = sprite->atlas->get_sub_texture(animator->currentFrame);
        }
    }
}