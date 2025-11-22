#include "resources/SpriteBatch.h"

#include "glad/glad.h"

#include "resources/RenderContext.h"
#include "resources/ShaderProgram.h"
#include "resources/Texture2D.h"

#include "components/core/Transform.h"
#include "components/rendering/Sprite.h"

#include <iostream>

SpriteBatch::SpriteBatch(std::shared_ptr<RenderContext> context, std::shared_ptr<ShaderProgram> shader, std::shared_ptr<Texture2D> texture)
: _context(std::move(context))
, _shader(std::move(shader))
, _texture(std::move(texture))
{
    _vao.bind();
    _context->quadEBO.bind();

    _vao.addBufferFloat(0, _context->quadVBO, 2, sizeof(float) * 4, 0);
    glVertexAttribDivisor(0, 0);
    _vao.addBufferFloat(1, _context->quadVBO, 2, sizeof(float) * 4, sizeof(float) * 2);
    glVertexAttribDivisor(1, 0);
    
    _vbo.init(GL_ARRAY_BUFFER, nullptr, 0, GL_DYNAMIC_DRAW);
    u_int64_t stride = sizeof(InstanceData);

    _vao.addBufferFloat(2, _vbo, 2, stride, offsetof(InstanceData, InstanceData::position));
    glVertexAttribDivisor(2, 1);

    _vao.addBufferFloat(3, _vbo, 2, stride, offsetof(InstanceData, InstanceData::size));
    glVertexAttribDivisor(3, 1);

    _vao.addBufferFloat(4, _vbo, 1, stride, offsetof(InstanceData, InstanceData::rotation));
    glVertexAttribDivisor(4, 1);

    _vao.addBufferFloat(5, _vbo, 2, stride, offsetof(InstanceData, InstanceData::lbUV));
    glVertexAttribDivisor(5, 1);

    _vao.addBufferFloat(6, _vbo, 2, stride, offsetof(InstanceData, InstanceData::rtUV));
    glVertexAttribDivisor(6, 1);

    _vao.addBufferByte(7, _vbo, 4, stride, offsetof(InstanceData, InstanceData::color));
    glVertexAttribDivisor(7, 1);

    _vao.addBufferFloat(8, _vbo, 1, stride, offsetof(InstanceData, InstanceData::layer));
    glVertexAttribDivisor(8, 1);
}

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
{
    _vao = std::move(other._vao);
    _instances = std::move(other._instances);
    _vbo = std::move(other._vbo);
    _context = other._context;
    _shader = std::move(other._shader);
    _texture = std::move(other._texture);
    _cacheCout = other._cacheCout;
    other._context = nullptr;
    other._cacheCout = 0;
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        _instances = std::move(other._instances);
        _vbo = std::move(other._vbo);
        _context = other._context;
        _shader = std::move(other._shader);
        _texture = std::move(other._texture);
        _cacheCout = other._cacheCout;
        other._context = nullptr;
        other._cacheCout = 0;
    }
    return *this;
}

SpriteBatch::~SpriteBatch()
{
    _instances.clear();
}

void SpriteBatch::beginBatch()
{
    _cacheCout = _instances.size();
    _instances.clear();
    _instances.reserve(_cacheCout);
}

void SpriteBatch::submit(Sprite2D *sprite, Transform *transform)
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

    _vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, _instances.size()*sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _instances.size()*sizeof(InstanceData), _instances.data());
    
    _shader->use();
    _texture->bind();
    
    _vao.bind();
    
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)_instances.size());
}

void SpriteBatch::flush()
{

}