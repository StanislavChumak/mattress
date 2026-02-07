#ifndef PRESETS_H
#define PRESETS_H

#include "comp/registerComponent.h"
#include "sys/SystemManager.h"

#ifdef INCLUDE_CORE_COMPONENTS
#include "comp/core/Name.h"
#include "comp/core/ParentChildren.h"
#include "comp/core/Transform.h"

#include "sys/transform/GlobalTransformSystem.h"
#endif

#define REGISTER_CORE_COMPONENTS(core) \
REGISTER_COMPONENT(Name); \
REGISTER_COMPONENT(Parent); \
REGISTER_COMPONENT(Children); \
REGISTER_COMPONENT(Transform); \
core.systems.register_updete<GlobalTransformSystem>(SystemPriority::TRANSFORM); \
core.systems.register_always_update<GlobalTransformSystem>()

#ifdef INCLUDE_SPRITE
#include "comp/single/Camera.h"
#include "comp/rendering/Sprite.h"

#include "sys/rendering/CameraSystem.h"
#include "sys/rendering/SpriteRenderSystem.h"
#endif

#define REGISTER_SPRITE(core) \
REGISTER_COMPONENT(Sprite2D); \
core.systems.register_updete<CameraSystem>(SystemPriority::UI_LOGIC); \
core.systems.register_always_update<CameraSystem>(); \
core.systems.register_updete<SpriteRenderSystem>(SystemPriority::RENDERING); \
core.systems.register_always_update<SpriteRenderSystem>()

#ifdef INCLUDE_ANIMATION
#include "comp/rendering/Animator.h"
#include "comp/rendering/StateAnimator.h"

#include "sys/rendering/AnimatorSystem.h"
#include "sys/rendering/StateAnimatorSystem.h"
#endif

#define REGISTER_ANIMATION(core) \
REGISTER_COMPONENT(Animator); \
REGISTER_COMPONENT(StateAnimator); \
core.systems.register_updete<AnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.register_always_update<AnimatorSystem>(); \
core.systems.register_updete<StateAnimatorSystem>(SystemPriority::ANIMATION); \
core.systems.register_always_update<StateAnimatorSystem>()

#ifdef INCLUDE_SOUNDS
#include "comp/single/Audio.h"
#include "comp/audio/Sound.h"
#include "comp/audio/Music.h"

#include "sys/audio/AudioSystem.h"
#endif

#define REGISTER_SOUNDS(core) \
REGISTER_COMPONENT(Sound); \
REGISTER_COMPONENT(Music); \
core.world.add_single_comp(Audio()); \
core.systems.register_updete<AudioSystem>(SystemPriority::AUDIO); \
core.systems.register_always_update<AudioSystem>()

#ifdef INCLUDE_LOCATION_SOUNDS
#include "comp/single/Listener.h"

#include "sys/audio/LocationAudioSystem.h"
#endif

#define REGISTER_LOCATION_SOUNDS(core) \
core.systems.register_updete<LocationAudioSystem>(SystemPriority::AUDIO); \
core.systems.register_always_update<LocationAudioSystem>(); \
core.world.add_single_comp(Listener())

#endif
