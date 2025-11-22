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

src/Engine.cpp
src/jsonUtils.cpp

src/systems/SystemManager.cpp

src/systems/input/InputSystem.cpp
src/systems/transform/CursorFollowersSystem.cpp
src/systems/transform/GlobalTransformSystem.cpp
src/systems/rendering/CameraSystem.cpp
src/systems/rendering/SpriteRenderSystem.cpp
src/systems/rendering/AnimatorSystem.cpp
src/systems/rendering/StateAnimatorSystem.cpp
src/systems/audio/AudioSystem.cpp
src/systems/audio/LocationAudioSystem.cpp
src/systems/cleanup/RemoveOrphans.cpp

src/resources/ResourceManager.cpp

src/resources/RenderContext.cpp
src/resources/SpriteBatch.cpp
src/resources/ShaderProgram.cpp
src/resources/Texture2D.cpp
src/resources/BufferObject.cpp
src/resources/VertexArrayObject.cpp


)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory (external/glad glad)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)

add_subdirectory(external/glfw glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_subdirectory(external/glm glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

add_subdirectory(external/simdjson)
target_link_libraries(${PROJECT_NAME} PUBLIC simdjson)
