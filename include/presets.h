#ifndef PRESETS_H
#define PRESETS_H

#include "components/registerComponent.h"
#include "systems/SystemManager.h"

#ifdef INCLUDE_CORE_COMPONENTS
#include "components/core/Name.h"
#include "components/core/ParentChildren.h"
#include "components/core/Transform.h"

#include "systems/transform/GlobalTransformSystem.h"
#endif

#define REGISTER_CORE_COMPONENTS(core) \
REGISTER_COMPONENT(Name); \
REGISTER_COMPONENT(Parent); \
REGISTER_COMPONENT(Children); \
REGISTER_COMPONENT(Transform); \
core.systems.registerUpdete<GlobalTransformSystem>(SystemPriority::TRANSFORM); \
core.systems.registerAlwaysUpdate<GlobalTransformSystem>()

#ifdef INCLUDE_SPRITE
#include "components/rendering/Camera.h"
#include "components/rendering/Sprite.h"

#include "systems/rendering/CameraSystem.h"
#include "systems/rendering/SpriteRenderSystem.h"
#endif

#define REGISTER_SPRITE(core) \
REGISTER_COMPONENT(Sprite2D); \
core.systems.registerUpdete<CameraSystem>(SystemPriority::UI_LOGIC); \
core.systems.registerAlwaysUpdate<CameraSystem>(); \
core.systems.registerUpdete<SpriteRenderSystem>(SystemPriority::RENDERING); \
core.systems.registerAlwaysUpdate<SpriteRenderSystem>()

#ifdef INCLUDE_ANIMATION
#include "components/rendering/Animator.h"
#include "components/rendering/StateAnimator.h"

#include "systems/rendering/AnimatorSystem.h"
#include "systems/rendering/StateAnimatorSystem.h"
#endif

#define REGISTER_ANIMATION(core) \
REGISTER_COMPONENT(Animator); \
REGISTER_COMPONENT(StateAnimator); \
core.systems.registerUpdete<AnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.registerAlwaysUpdate<AnimatorSystem>(); \
core.systems.registerUpdete<StateAnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.registerAlwaysUpdate<StateAnimatorSystem>()

#ifdef INCLUDE_SOUNDS
#include "components/audio/Audio.h"
#include "components/audio/Sound.h"
#include "components/audio/Music.h"

#include "systems/audio/AudioSystem.h"
#endif

#define REGISTER_SOUNDS(core) \
REGISTER_COMPONENT(Sound); \
REGISTER_COMPONENT(Music); \
core.world.addSingleComponent(Audio()); \
core.systems.registerUpdete<AudioSystem>(SystemPriority::AUDIO); \
core.systems.registerAlwaysUpdate<AudioSystem>()

#ifdef INCLUDE_LOCATION_SOUNDS
#include "components/audio/Listener.h"

#include "systems/audio/LocationAudioSystem.h"
#endif

#define REGISTER_LOCATION_SOUNDS(core) \
core.systems.registerUpdete<LocationAudioSystem>(SystemPriority::AUDIO); \
core.systems.registerAlwaysUpdate<LocationAudioSystem>(); \
core.world.addSingleComponent(Listener())

#endif
