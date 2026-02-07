#include "sys/rendering/CameraSystem.h"

#include "comp/ECSWorld.h"
#include "comp/single/Camera.h"
#include "comp/core/Transform.h"

void CameraSystem::update(ECSWorld &world, const double &delta)
{
    Camera *camera = world.get_single_comp<Camera>();
    if(!camera || camera->target == NULL_ENTITY) return;
    Transform *transform = world.get_comp<Transform>(camera->target);
    if(!transform) return;
 
    camera->update_view_matrix(glm::inverse(transform->globalMatrix));
}