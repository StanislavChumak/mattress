cmake_minimum_required(VERSION 3.13.0 FATAL_ERROR)

set(PROJECT_NAME Engine)
project(${PROJECT_NAME} VERSION 1.0.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()


add_library(${PROJECT_NAME} STATIC

external/stb_image.h
external/miniaudio.h

include/Engine.h
src/Engine.cpp
src/ECSWorld.h
src/View.h
src/SparseSet.h
src/jsonManager.h
src/jsonManager.cpp


src/systems/SystemManager.h
src/systems/SystemManager.cpp
include/SystemBase.h

src/systems/input/InputSystem.h
src/systems/input/InputSystem.cpp
src/systems/input/CursorFollowersSystem.h
src/systems/input/CursorFollowersSystem.cpp
src/systems/rendering/RenderSystem.h
src/systems/rendering/RenderSystem.cpp
src/systems/rendering/AnimatorSystem.h
src/systems/rendering/AnimatorSystem.cpp


include/components/registerComponent.h

include/components/core/Transform.h
include/components/core/Input.h
include/components/core/Window.h
include/components/core/Game.h
include/components/rendering/Camera.h
include/components/rendering/Animator.h
include/components/rendering/Sprite.h
include/components/audio/Audio.h
include/components/ui/Cursor.h



src/resources/ResourceManager.h
src/resources/ResourceManager.cpp

src/resources/renderer/ShaderProgram.h
src/resources/renderer/ShaderProgram.cpp
src/resources/renderer/Texture2D.h
src/resources/renderer/Texture2D.cpp
src/resources/renderer/BufferObject.h
src/resources/renderer/BufferObject.cpp
src/resources/renderer/VertexArrayObject.h
src/resources/renderer/VertexArrayObject.cpp

src/resources/sound/Sound.h
src/resources/sound/Sound.cpp

)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/rapidjson/include
)

add_subdirectory (external/glad glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(external/glfw glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(external/glm glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

include_directories(external/rapidjson/include)
