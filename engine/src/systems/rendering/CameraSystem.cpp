#include "systems/rendering/CameraSystem.h"

#include "ECSWorld.h"

#include "components/rendering/Camera.h"
#include "components/core/Transform.h"

void CameraSystem::update(ECSWorld &world, const double &delta)
{
    Camera *camera = world.getSingleComponent<Camera>();
    if(!camera || camera->target == NULL_ENTITY) return;
    Transform *transform = world.getComponent<Transform>(camera->target);
    if(!transform) return;
 
    camera->updateViewMatrix(glm::inverse(transform->globalMatrix));
}