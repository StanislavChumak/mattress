#include "AnimatorSystem.h"

#include "components/rendering/Sprite.h"
#include "components/rendering/Animator.h"
#include "../../ECSWorld.h"

void AnimatorSystem::update(ECSWorld &world, const double &delta)
{
    for(auto [entity, sprite, animator] : world.view<Sprite2D, Animator>())
    {
        animator->currentAnimationTime += delta;

        while (animator->currentAnimationTime >= animator->framesDuration[animator->currentFrame])
        {
            animator->currentAnimationTime -= animator->framesDuration[animator->currentFrame];
            ++animator->currentFrame;

            if (animator->currentFrame == animator->framesCount)
            {
                animator->currentFrame = 0;
            }

            const Texture2D::SubTexture2D &nextSubTexture = sprite->subTextures[animator->currentFrame];

            const GLfloat textureCoords[] = {
            nextSubTexture.rightTopVertex.x, nextSubTexture.rightTopVertex.y,    // right top
            nextSubTexture.rightTopVertex.x, nextSubTexture.leftBottomVertex.y,  // right bottom
            nextSubTexture.leftBottomVertex.x, nextSubTexture.rightTopVertex.y,  // left top
            nextSubTexture.leftBottomVertex.x, nextSubTexture.leftBottomVertex.y // left bottom
            };
            sprite->VBOtex.update(textureCoords, sizeof(textureCoords), 0);
        }
    }
}