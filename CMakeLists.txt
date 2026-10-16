cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

project(MattressEngine)
set(LIB_NAME "mattress")

option(MATTRESS_BUILD_APP "Build the engine as a standalone application executable" OFF)
if(NOT DEFINED MATTRESS_APP_NAME)
    set(MATTRESS_APP_NAME "mattress_app")
endif()

# Engine configuration. The consuming project (game) may override these before
# add_subdirectory(engine), or they can be set with -D on the command line.
set(ENGINE_WINDOW_NAME         "MattressProject" CACHE STRING "Window title")
set(ENGINE_PACK_DIR            "./"              CACHE STRING "Runtime pack directory")
set(ENGINE_SCENE_DIR           "./"              CACHE STRING "Runtime scene directory")
set(ENGINE_START_SCENE         ""                CACHE STRING "Scene loaded on startup")
set(ENGINE_ICON_PATHS          ""                CACHE STRING "Window icons (CMake list), relative to the executable")
set(ENGINE_ICON_COUNT          0                 CACHE STRING "Count window icon")
set(ENGINE_WINDOW_SIZE_X       1024              CACHE STRING "Window width in points")
set(ENGINE_WINDOW_SIZE_Y       640               CACHE STRING "Window height in points")
set(ENGINE_START_POINT_SIZE    1                 CACHE STRING "Pixel scale")
set(ENGINE_MAX_FPS             60                CACHE STRING "Target frame rate")
set(ENGINE_PACKS_CACHE_LIMIT   15                CACHE STRING "Packs cache limit in KB")
set(ENGINE_SCENES_CACHE_LIMIT  50                CACHE STRING "Scenes cache limit in KB")
set(ENGINE_SOUND_LOCATION_SCALE 1                CACHE STRING "Sound location scale")
set(ENGINE_DISPLAY_CURSOR      ON                CACHE BOOL   "Display the cursor")
set(ENGINE_BLEND               ON                CACHE BOOL   "Enable blending")
set(ENGINE_FIXED_HORIZONTAL    ON                CACHE BOOL   "Fix horizontal camera side")
set(ENGINE_FIXED_VERTICAL      ON                CACHE BOOL   "Fix vertical camera side")
set(ENGINE_CLEAR_COLOR_R       0.2               CACHE STRING "Clear color red")
set(ENGINE_CLEAR_COLOR_G       0.0               CACHE STRING "Clear color green")
set(ENGINE_CLEAR_COLOR_B       0.3               CACHE STRING "Clear color blue")
set(ENGINE_CLEAR_COLOR_A       1.0               CACHE STRING "Clear color alpha")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release optimization. -march=native tunes the binary for the machine that
# builds it, so the result may crash with "illegal instruction" on an older CPU.
# Ships as OFF (portable, generic tuning); enable for local/per-machine builds.
option(ENGINE_NATIVE_OPT "Optimize the release build for the build machine's CPU (-march=native)" OFF)
if(ENGINE_NATIVE_OPT)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -mtune=generic")
endif()

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()

add_subdirectory(external/utils mtrsutils)
add_subdirectory(external/glad glad)
add_subdirectory(external/glfw glfw)
add_subdirectory(external/glm glm)

add_library(${LIB_NAME} STATIC

src/Engine.cpp

# Systems
src/sys/SystemManager.cpp

src/sys/core/InputSystem.cpp
src/sys/core/InputSybscrubersSystem.cpp
src/sys/core/ScriptsSystem.cpp
src/sys/core/TransformSystem.cpp

src/sys/rendering/CameraSystem.cpp
src/sys/rendering/SpriteSubmitSystem.cpp
src/sys/rendering/AnimatorSystem.cpp
src/sys/rendering/StateAnimatorSystem.cpp
src/sys/rendering/SpriteMapSubmitSystem.cpp
src/sys/rendering/MapAnimatorSystem.cpp
src/sys/rendering/TextSubmitSystem.cpp
src/sys/rendering/RenderSystem.cpp

src/sys/audio/LocationAudioSystem.cpp

# Resourses
src/res/ResourceManager.cpp

src/res/asset/ScriptFile.cpp
src/res/asset/ShaderProgram.cpp
src/res/asset/Texture.cpp
src/res/asset/TextureAtlas.cpp
src/res/asset/Sound.cpp
src/res/asset/Font.cpp

src/res/render/SpriteBatch.cpp
src/res/render/BufferObject.cpp
src/res/render/VertexArrayObject.cpp

# Components
src/comp/ECSWorld.cpp

src/comp/collections/ComponentManager.cpp

src/comp/single/Window.cpp
src/comp/single/Camera.cpp
src/comp/single/Render.cpp
src/comp/single/GlyphDecoder.cpp
src/comp/single/Cursor.cpp
src/comp/single/KeyButtons.cpp
src/comp/single/MouseButtons.cpp
src/comp/single/MouseScroll.cpp
src/comp/single/Audio.cpp
src/comp/single/Listener.cpp

src/comp/core/ScriptUpdate.cpp
src/comp/core/ScriptCallback.cpp
src/comp/core/StoredData.cpp
src/comp/core/Transform.cpp

src/comp/rendering/Sprite.cpp
src/comp/rendering/Animator.cpp
src/comp/rendering/StateAnimator.cpp
src/comp/rendering/SpriteMap.cpp
src/comp/rendering/MapAnimator.cpp

src/comp/ui/Label.cpp

src/comp/audio/SoundPlayer.cpp

)

set_property(TARGET ${LIB_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<CONFIG:Release>:FLAG_RELEASE>
)

set(_engine_icon_paths_def "")
foreach(_engine_icon_path IN LISTS ENGINE_ICON_PATHS)
    if(NOT _engine_icon_paths_def STREQUAL "")
        string(APPEND _engine_icon_paths_def ",")
    endif()
    string(APPEND _engine_icon_paths_def "\"${_engine_icon_path}\"")
endforeach()
if(_engine_icon_paths_def STREQUAL "")
    set(_engine_icon_paths_def "\"\"")
endif()

target_compile_definitions(${LIB_NAME} PUBLIC
    ENGINE_WINDOW_NAME="${ENGINE_WINDOW_NAME}"
    ENGINE_PACK_DIR="${ENGINE_PACK_DIR}"
    ENGINE_SCENE_DIR="${ENGINE_SCENE_DIR}"
    ENGINE_START_SCENE="${ENGINE_START_SCENE}"
    ENGINE_ICON_PATHS=${_engine_icon_paths_def}
    ENGINE_ICON_COUNT=${ENGINE_ICON_COUNT}
    ENGINE_WINDOW_SIZE_X=${ENGINE_WINDOW_SIZE_X}
    ENGINE_WINDOW_SIZE_Y=${ENGINE_WINDOW_SIZE_Y}
    ENGINE_START_POINT_SIZE=${ENGINE_START_POINT_SIZE}
    ENGINE_MAX_FPS=${ENGINE_MAX_FPS}
    ENGINE_PACKS_CACHE_LIMIT=${ENGINE_PACKS_CACHE_LIMIT}
    ENGINE_SCENES_CACHE_LIMIT=${ENGINE_SCENES_CACHE_LIMIT}
    ENGINE_SOUND_LOCATION_SCALE=${ENGINE_SOUND_LOCATION_SCALE}
    ENGINE_DISPLAY_CURSOR=$<BOOL:${ENGINE_DISPLAY_CURSOR}>
    ENGINE_BLEND=$<BOOL:${ENGINE_BLEND}>
    ENGINE_FIXED_HORIZONTAL=$<BOOL:${ENGINE_FIXED_HORIZONTAL}>
    ENGINE_FIXED_VERTICAL=$<BOOL:${ENGINE_FIXED_VERTICAL}>
    ENGINE_CLEAR_COLOR_R=${ENGINE_CLEAR_COLOR_R}
    ENGINE_CLEAR_COLOR_G=${ENGINE_CLEAR_COLOR_G}
    ENGINE_CLEAR_COLOR_B=${ENGINE_CLEAR_COLOR_B}
    ENGINE_CLEAR_COLOR_A=${ENGINE_CLEAR_COLOR_A}
)

target_include_directories(${LIB_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/single-files
)

target_link_libraries(${LIB_NAME} PUBLIC mtrsutils)
target_link_libraries(${LIB_NAME} PUBLIC glad)
target_link_libraries(${LIB_NAME} PUBLIC glfw)
target_link_libraries(${LIB_NAME} PUBLIC glm)

if(MATTRESS_BUILD_APP)
    add_executable(${MATTRESS_APP_NAME} src/Main.cpp)
    target_link_libraries(${MATTRESS_APP_NAME} PRIVATE ${LIB_NAME})
    set_target_properties(${MATTRESS_APP_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ENABLE_EXPORTS ON
    )
    if(WIN32)
        set_target_properties(${MATTRESS_APP_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
    set(MATTRESS_APP_TARGET ${MATTRESS_APP_NAME} PARENT_SCOPE)
endif()
