#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include "BufferObject.h"
#include "VertexArrayObject.h"

#include <vector>
#include <memory>

class ShaderProgram;
class Texture;

class RenderContext;

struct InstanceData
{
    glm::vec2 position;
    glm::vec2 size;
    float rotation;
    uint32_t _pad;
    glm::vec2 lbUV;
    glm::vec2 rtUV;
    glm::tvec4<uint8_t, glm::precision::highp> color;
    float layer;
};

class SpriteBatch
{
    friend class RenderContext;
private:
    static constexpr uint8_t BUFFER_COUNT = 3;
    static constexpr uint32_t MAX_INSTANCES = 1000;

    std::vector<InstanceData> _instances;
    VertexArrayObject _vao;

    BufferObject _instanceVBO[BUFFER_COUNT];
    InstanceData* _mappedBuffers[BUFFER_COUNT] = {nullptr};
    GLsync _fences[BUFFER_COUNT] = {0};
    uint8_t _currentBufferIndex = 0;
    
    std::shared_ptr<ShaderProgram> _shader;
    std::shared_ptr<Texture> _texture;
    
public:
    SpriteBatch() = default;
    SpriteBatch(const SpriteBatch&) = delete;
    SpriteBatch &operator=(const SpriteBatch&) = delete;
    SpriteBatch(SpriteBatch &&other) noexcept;
    SpriteBatch &operator=(SpriteBatch &&other) noexcept;
    ~SpriteBatch() noexcept;

    void begin_batch();
    void submit(InstanceData date);
    void end_batch();
    void flush();
};

#endif
