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

src/sys/SystemManager.cpp

src/sys/input/InputSystem.cpp
src/sys/transform/CursorFollowersSystem.cpp
src/sys/transform/GlobalTransformSystem.cpp
src/sys/rendering/CameraSystem.cpp
src/sys/rendering/SpriteRenderSystem.cpp
src/sys/rendering/AnimatorSystem.cpp
src/sys/rendering/StateAnimatorSystem.cpp
src/sys/audio/AudioSystem.cpp
src/sys/audio/LocationAudioSystem.cpp
src/sys/cleanup/RemoveOrphans.cpp

src/res/ResourceManager.cpp

src/res/RenderContext.cpp
src/res/SpriteBatch.cpp
src/res/ShaderProgram.cpp
src/res/Texture2D.cpp
src/res/TextureAtlas.cpp
src/res/BufferObject.cpp
src/res/VertexArrayObject.cpp
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
