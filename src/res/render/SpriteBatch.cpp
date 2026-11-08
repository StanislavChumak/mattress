#include "res/render/SpriteBatch.hpp"

#include "glad/glad.h"

#include <cstddef>
#include <cstring>

#include "util/fun/msg/mtrs_message.hpp"

namespace mtrs::res
{

SpriteBatch::SpriteBatch(const BufferObject &quad_VBO, const BufferObject &quad_EBO,
    uint32_t shader, uint32_t texture, uint64_t max_instances)
: _shader(shader), _texture(texture)
, _max_instances(max_instances ? max_instances : 1)
{
    _instance_vbo.init(GL_ARRAY_BUFFER, nullptr,
        _max_instances * sizeof(InstanceData), GL_DYNAMIC_DRAW);

    _vao.bind();

    _vao.set_vertex_buffer(quad_VBO);
    _vao.add_attrib_float(0, 2, sizeof(float) * 4, 0);
    _vao.add_attrib_float(1, 2, sizeof(float) * 4, sizeof(float) * 2);

    _vao.set_vertex_buffer(_instance_vbo);
    _vao.add_attrib_float(2, 2, sizeof(InstanceData), offsetof(InstanceData, position));
    _vao.add_attrib_float(3, 2, sizeof(InstanceData), offsetof(InstanceData, size));
    _vao.add_attrib_float(4, 1, sizeof(InstanceData), offsetof(InstanceData, rotation));
    _vao.add_attrib_float(5, 2, sizeof(InstanceData), offsetof(InstanceData, lb_uv));
    _vao.add_attrib_float(6, 2, sizeof(InstanceData), offsetof(InstanceData, rt_uv));
    _vao.add_attrib_byteN(7, 4, sizeof(InstanceData), offsetof(InstanceData, color));

    for(GLuint attrib = 2; attrib <= 7; attrib++)
    {
        _vao.set_divisor(attrib, 1);
    }

    _vao.set_index_buffer(quad_EBO);
    _vao.unbind();
}

SpriteBatch::SpriteBatch(SpriteBatch &&other) noexcept
: _vao(std::move(other._vao))
, _instance_vbo(std::move(other._instance_vbo))
, _shader(other._shader)
, _texture(other._texture)
, _max_instances(other._max_instances)
{
    instances = std::move(other.instances);
    layers = std::move(other.layers);

    other._shader = 0;
    other._texture = 0;
    other._max_instances = 0;
}

SpriteBatch &SpriteBatch::operator=(SpriteBatch &&other) noexcept
{
    if(this != &other)
    {
        _vao = std::move(other._vao);
        _instance_vbo = std::move(other._instance_vbo);
        instances = std::move(other.instances);
        layers = std::move(other.layers);

        _shader = other._shader;
        _texture = other._texture;
        _max_instances = other._max_instances;

        other._shader = 0;
        other._texture = 0;
        other._max_instances = 0;
    }
    return *this;
}

SpriteBatch::~SpriteBatch() noexcept
{
    instances.clear();
    layers.clear();
}

void SpriteBatch::begin_batch()
{
}

void SpriteBatch::draw_layer(float layer)
{
    if(instances.empty()) return;

    auto iter = instances.find(layer);
    if(iter == instances.end() || iter->second.empty()) return;

    const std::vector<InstanceData> &data = iter->second;

#ifndef FLAG_RELEASE
    if(data.size() > _max_instances)
    {
        msg::mtrs_warning("The number of instances is ", data.size(),
            ", which has exceeded the MAX_INSTANCES limit of ", _max_instances,
            " in the SpriteBatch resource.");
    }
#endif

    if(data.size() > _max_instances)
    {
        _max_instances = data.size();
        _instance_vbo.init(GL_ARRAY_BUFFER, nullptr,
            _max_instances * sizeof(InstanceData), GL_DYNAMIC_DRAW);
    }

    const GLsizeiptr bytes = static_cast<GLsizeiptr>(data.size() * sizeof(InstanceData));

    _vao.bind();
    _instance_vbo.bind();

    void *mapped = glMapBufferRange(GL_ARRAY_BUFFER, 0, bytes,
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if(mapped)
    {
        std::memcpy(mapped, data.data(), static_cast<size_t>(bytes));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, data.data());
    }

    glUseProgram(_shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
        static_cast<GLsizei>(data.size()));
}

void SpriteBatch::end_batch()
{
    instances.clear();
    layers.clear();
}

}
