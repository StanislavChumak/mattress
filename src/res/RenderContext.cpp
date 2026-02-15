#include "res/RenderContext.h"

#include "glad/glad.h"

#include "res/ShaderProgram.h"
#include "res/Texture.h"

RenderContext::RenderContext()
{
    const float quad[] = {
        -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };
    _quadVBO.init(GL_ARRAY_BUFFER, quad, sizeof(quad), GL_STATIC_DRAW);
    
    const GLuint indices[] = {0, 1, 2, 2, 3, 0};
    _quadEBO.init(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices), GL_STATIC_DRAW);
}

RenderContext::RenderContext(RenderContext &&other) noexcept
{
    _quadEBO = std::move(other._quadEBO);
    _quadVBO = std::move(other._quadVBO);
    _batches = std::move(other._batches);
}

RenderContext &RenderContext::operator=(RenderContext &&other) noexcept
{
    if(this != &other)
    {
        _quadEBO = std::move(other._quadEBO);
        _quadVBO = std::move(other._quadVBO);
        _batches = std::move(other._batches);
    }
    return *this;
}

void RenderContext::create_sprite_batch(std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture2D> texture)
{
    u_int64_t id = shader->id() | u_int64_t(texture->id()) << 32;

    if (_batches.find(id) != _batches.end()) return;

    SpriteBatch &batch = _batches.emplace(id, SpriteBatch()).first->second;
    _keys.push_back(id);

    batch._shader = std::move(shader);
    batch._texture = std::move(texture);

    for(u_char i = 0; i < SpriteBatch::BUFFER_COUNT; i++)
    {
        glGenBuffers(1, &batch._instanceVBO[i]._id);
        glBindBuffer(GL_ARRAY_BUFFER, batch._instanceVBO[i]._id);
        glBufferStorage(GL_ARRAY_BUFFER, SpriteBatch::MAX_INSTANCES * sizeof(InstanceData), nullptr,
                                   GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

        batch._mappedBuffers[i] = reinterpret_cast<InstanceData*>(glMapBufferRange(
                                  GL_ARRAY_BUFFER, 0, SpriteBatch::MAX_INSTANCES * sizeof(InstanceData),
                                  GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));
    }

    batch._vao.bind();
    _quadEBO.bind();

    _quadVBO.bind();

    batch._vao.add_buffer_float(0, _quadVBO, 2, sizeof(float)*4, 0);
    batch._vao.add_buffer_float(1, _quadVBO, 2, sizeof(float)*4, sizeof(float)*2);

    BufferObject &vbo = batch._instanceVBO[0];
    vbo.bind();

    batch._vao.add_buffer_float(2, vbo, 2, sizeof(InstanceData), offsetof(InstanceData, position));
    batch._vao.add_buffer_float(3, vbo, 2, sizeof(InstanceData), offsetof(InstanceData, size));
    batch._vao.add_buffer_float(4, vbo, 1, sizeof(InstanceData), offsetof(InstanceData, rotation));
    batch._vao.add_buffer_float(5, vbo, 2, sizeof(InstanceData), offsetof(InstanceData, lbUV));
    batch._vao.add_buffer_float(6, vbo, 2, sizeof(InstanceData), offsetof(InstanceData, rtUV));
    batch._vao.add_buffer_byteN(7, vbo, 4, sizeof(InstanceData), offsetof(InstanceData, color));
    batch._vao.add_buffer_float(8, vbo, 1, sizeof(InstanceData), offsetof(InstanceData, layer));

    for (int i = 2; i <= 8; i++)
        glVertexAttribDivisor(i, 1);

    batch._vao.unbind();
}

void RenderContext::begin_batches()
{
    for(auto &key : _keys)
    {
        _batches.at(key).begin_batch();
    }
}

void RenderContext::submit_batch(uint64_t id, InstanceData date)
{
    _batches.at(id).submit(std::move(date));
}

void RenderContext::end_batches()
{
    for(auto &key : _keys)
    {
        _batches.at(key).end_batch();
    }
}