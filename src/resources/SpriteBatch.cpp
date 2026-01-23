#include "resources/SpriteBatch.h"

#include "glad/glad.h"

#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    _instances = std::move(other._instances);
    for(u_char i = 0; i < BUFFER_COUNT; i++)
    {
        _instanceVBO[i] = std::move(other._instanceVBO[i]);
        _mappedBuffers[i] = other._mappedBuffers[i];
        _fences[i] = other._fences[i];
        other._mappedBuffers[i] = nullptr;
        other._fences[i] = 0;
    }
    _shader = std::move(other._shader);
    _texture = std::move(other._texture);
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        _instances = std::move(other._instances);
        for(u_char i = 0; i < BUFFER_COUNT; i++)
        {
            _instanceVBO[i] = std::move(other._instanceVBO[i]);
            _mappedBuffers[i] = other._mappedBuffers[i];
            _fences[i] = other._fences[i];
            other._mappedBuffers[i] = nullptr;
            other._fences[i] = 0;
        }
        _shader = std::move(other._shader);
        _texture = std::move(other._texture);
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    _instances.clear();
}

void SpriteBatch::beginBatch()
{
    size_t cacheCout = _instances.size();
    _instances.clear();
    _instances.reserve(cacheCout);
}

void SpriteBatch::submit(Transform *transform, Sprite2D *sprite)
{
    InstanceData date;
    glm::mat4 glob = transform->globalMatrix;

    date.position = glm::vec2(glob[3].x, glob[3].y);

    glm::vec2 scale;
    scale.x = glm::length(glm::vec2(glob[0].x, glob[0].y));
    scale.y = glm::length(glm::vec2(glob[1].x, glob[1].y));
    date.size = sprite->size * scale;

    date.rotation = std::atan2(glob[0].y, glob[0].x);

    date.lbUV = sprite->subTexture.lbVertex;
    date.rtUV = sprite->subTexture.rtVertex;
    date.color = sprite->color;
    date.layer = sprite->layer;

    _instances.push_back(date);
}

void SpriteBatch::endBatch()
{
    if(_instances.empty()) return;

    ++_currentBufferIndex %= BUFFER_COUNT;

    if(_fences[_currentBufferIndex])
    {
        GLenum result = glClientWaitSync(_fences[_currentBufferIndex], GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
        if (result != GL_ALREADY_SIGNALED && result != GL_CONDITION_SATISFIED)
        {
            glWaitSync(_fences[_currentBufferIndex], 0, GL_TIMEOUT_IGNORED);
        }
        glDeleteSync(_fences[_currentBufferIndex]);
        _fences[_currentBufferIndex] = 0;
    }

    InstanceData *ptrVbo = _mappedBuffers[_currentBufferIndex];
    std::memcpy(ptrVbo, _instances.data(), _instances.size() * sizeof(InstanceData));
    
    _vao.bind();
    _instanceVBO[_currentBufferIndex].bind();
    
    _shader->use();
    _texture->bind();
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)_instances.size());

    _fences[_currentBufferIndex] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void SpriteBatch::flush()
{

}